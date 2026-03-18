#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edit.h"

/*
 * write_syncsafe()
 * ----------------
 * Writes a 32-bit integer as a sync-safe integer (ID3v2.4 format)
 * Sync-safe means MSB of each byte is zero
 */
void write_syncsafe(FILE *fp, unsigned int size)
{
    unsigned char b[4];

    b[0] = (size >> 21) & 0x7F;   // highest 7 bits
    b[1] = (size >> 14) & 0x7F;
    b[2] = (size >> 7)  & 0x7F;
    b[3] = size & 0x7F;           // lowest 7 bits

    fwrite(b, 1, 4, fp);          // write 4 bytes to file
}

/*
 * read_frame_size()
 * -----------------
 * Reads frame size from MP3 file
 * Supports both:
 *   - Sync-safe (ID3v2.4)
 *   - Big-endian (ID3v2.3)
 */
unsigned int read_frame_size(FILE *fp)
{
    unsigned char sz[4];
    fread(sz, 1, 4, fp);          // read 4 bytes size field

    // Check if all MSBs are 0 → sync-safe size
    if ((sz[0] & 0x80) == 0 &&
        (sz[1] & 0x80) == 0 &&
        (sz[2] & 0x80) == 0 &&
        (sz[3] & 0x80) == 0)
    {
        return (sz[0] << 21) |
               (sz[1] << 14) |
               (sz[2] << 7)  |
                sz[3];
    }
    // Otherwise treat as big-endian size
    else
    {
        return (sz[0] << 24) |
               (sz[1] << 16) |
               (sz[2] << 8)  |
                sz[3];
    }
}

/*
 * edit_tag()
 * ----------
 * Edits a specific ID3 frame (like TIT2, TPE1, TALB)
 * filename  : MP3 file name
 * frame_id  : Frame to edit
 * new_data  : New text to write into frame
 */
void edit_tag(const char *filename, const char *frame_id, const char *new_data)
{
    FILE *fps, *fpt;
    char header[10];                  // ID3 header
    char tag[5];                      // Frame ID (4 chars + '\0')
    unsigned int tag_size;            // Updated tag size
    unsigned int original_tag_size;   // Original tag size
    unsigned int actual_size;         // Frame data size
    char flags[2];                    // Frame flags
    char *data;
    int edited = 0;                   // Flag to ensure single edit
    char ch;

    // Open original MP3 file
    fps = fopen(filename, "rb");
    if (!fps)
    {
        printf("Error: Cannot open file\n");
        return;
    }

    // Create temporary MP3 file
    fpt = fopen("temp.mp3", "wb");
    if (!fpt)
    {
        fclose(fps);
        printf("Error: Cannot create temp file\n");
        return;
    }

    /* ---------- READ ID3 HEADER ---------- */
    fread(header, 1, 10, fps);         // Read 10-byte ID3 header

    // Extract tag size (sync-safe)
    tag_size = ((header[6] & 0x7F) << 21) |
               ((header[7] & 0x7F) << 14) |
               ((header[8] & 0x7F) << 7)  |
               (header[9] & 0x7F);

    original_tag_size = tag_size;      // Save original tag size

    fwrite(header, 1, 10, fpt);        // Copy header to temp file

    /* ---------- FRAME PROCESSING ---------- */
    while (ftell(fps) < original_tag_size + 10)
    {
        // Read frame ID
        if (fread(tag, 1, 4, fps) != 4)
            break;

        tag[4] = '\0';                 // Null terminate frame ID

        actual_size = read_frame_size(fps);  // Read frame size
        fread(flags, 1, 2, fps);       // Read frame flags

        if (actual_size == 0)
            break;

        /* ---------- EDIT TARGET FRAME ---------- */
        if (!edited && strcmp(tag, frame_id) == 0)
        {
            unsigned int new_size = strlen(new_data) + 1; // +1 for encoding byte

            tag_size += (new_size - actual_size); // Update tag size

            fwrite(tag, 1, 4, fpt);                // Write frame ID
            write_syncsafe(fpt, new_size);         // Write new size
            fwrite(flags, 1, 2, fpt);              // Write flags

            ch = 0x00;                             // Text encoding (ISO-8859-1)
            fwrite(&ch, 1, 1, fpt);
            fwrite(new_data, 1, new_size - 1, fpt);

            fseek(fps, actual_size, SEEK_CUR);    // Skip old frame data
            edited = 1;                            // Mark frame edited
        }
        else
        {
            /* ---------- COPY FRAME AS IS ---------- */
            fwrite(tag, 1, 4, fpt);
            write_syncsafe(fpt, actual_size);
            fwrite(flags, 1, 2, fpt);

            data = (char *)malloc(actual_size);
            fread(data, 1, actual_size, fps);
            fwrite(data, 1, actual_size, fpt);
            free(data);
        }
    }

    /* ---------- UPDATE TAG HEADER SIZE ---------- */
    fseek(fpt, 6, SEEK_SET);           // Move to size field in header

    header[6] = (tag_size >> 21) & 0x7F;
    header[7] = (tag_size >> 14) & 0x7F;
    header[8] = (tag_size >> 7)  & 0x7F;
    header[9] = tag_size & 0x7F;

    fwrite(&header[6], 1, 4, fpt);     // Write updated size

    /* ---------- COPY AUDIO DATA ---------- */
    fseek(fps, original_tag_size + 10, SEEK_SET);
    fseek(fpt, tag_size + 10, SEEK_SET);

    while (fread(&ch, 1, 1, fps) == 1)
        fwrite(&ch, 1, 1, fpt);

    fclose(fps);
    fclose(fpt);

    remove(filename);                  // Delete original file
    rename("temp.mp3", filename);      // Rename temp file

    printf("Edit completed successfully\n");
}
