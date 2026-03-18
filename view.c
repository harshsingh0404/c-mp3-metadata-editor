#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

/*
 * view_tag()
 * ----------
 * Displays basic ID3v2 tag information from an MP3 file
 * filename : name of the MP3 file
 */
void view_tag(const char *filename)
{
    char tag[5];              // Frame ID (4 chars + null)
    unsigned int size;        // Frame data size
    char *data;               // Frame data buffer

    printf("view mode started\n");

    // Open MP3 file in binary read mode
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("Error File can't open\n");
        return;
    }

    printf("File opened Successfully\n");

    /* Skip ID3 header (10 bytes) */
    fseek(fp, 10, SEEK_SET);

    /* Read first few frames (limited to 6 frames) */
    for (int i = 0; i < 6; i++)
    {
        /* Read frame ID */
        if (fread(tag, 1, 4, fp) != 4)
            break;

        tag[4] = '\0';         // Null terminate frame ID

        /* Read sync-safe frame size */
        unsigned char sz[4];
        fread(sz, 1, 4, fp);

        size = (sz[0] << 21) |
               (sz[1] << 14) |
               (sz[2] << 7)  |
                sz[3];

        /* Stop if padding or invalid frame */
        if (size == 0)
            break;

        /* Skip frame flags (2 bytes) */
        fseek(fp, 2, SEEK_CUR);

        /* Allocate memory for frame data */
        data = (char *)malloc(size + 1);
        if (!data)
            break;

        /* Read frame data */
        fread(data, 1, size, fp);
        data[size] = '\0';     // Null terminate data

        /* Display known text frames (skip encoding byte) */
        if (strcmp(tag, "TIT2") == 0)
            printf("TITLE   : %s\n", data + 1);
        else if (strcmp(tag, "TPE1") == 0)
            printf("ARTIST  : %s\n", data + 1);
        else if (strcmp(tag, "TALB") == 0)
            printf("ALBUM   : %s\n", data + 1);
        else if (strcmp(tag, "TDRC") == 0 || strcmp(tag, "TYER") == 0)
            printf("YEAR    : %s\n", data + 1);
        else if (strcmp(tag, "TCON") == 0)
            printf("CONTENT : %s\n", data + 1);
        else if (strcmp(tag, "COMM") == 0)
            printf("COMMENT : %s\n", data + 1);

        /* Free allocated memory */
        free(data);
    }

    /* Close MP3 file */
    fclose(fp);
}
