/******************************************************************************
 * Project Name : MP3 Tag Editor
 * Description  : This file contains the main function which controls
 *                view and edit operations of ID3v2 tags in an MP3 file.
 *
 * Features     :
 *   - View MP3 metadata (Title, Artist, Album, Year, Genre, Comment)
 *   - Edit specific ID3v2 frames using command-line options
 *
 * Usage        :
 *   View Mode :
 *     ./mp3tag -v <file.mp3>
 *
 *   Edit Mode :
 *     ./mp3tag -e -t/-a/-A/-y/-m/-c <file.mp3> <new_data>
 *
 * Name         : Harsh Singh
 * Date         : 04/02/2026
 * 
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{
    /* ---------- CHECK MINIMUM ARGUMENTS ---------- */
    if (argc < 3)
    {
        printf("Usage:\n");
        printf("./mp3tag -v <file.mp3>\n");
        printf("./mp3tag -e <option> <file.mp3> <new_data>\n");
        return 1;
    }

    /* ---------- VIEW MODE ---------- */
    if (strcmp(argv[1], "-v") == 0)
    {
        // Check correct argument count for view mode
        if (argc != 3)
        {
            printf("Usage: ./mp3tag -v <file.mp3>\n");
            return 1;
        }

        // Call function to display MP3 tags
        view_tag(argv[2]);
    }

    /* ---------- EDIT MODE ---------- */
    else if (strcmp(argv[1], "-e") == 0)
    {
        // Check correct argument count for edit mode
        if (argc != 5)
        {
            printf("Usage: ./mp3tag -e -t/-a/-A/-y/-m/-c <file.mp3> <new_data>\n");
            return 1;
        }

        char frame_id[5];   // Stores ID3 frame ID

        /* ---------- OPTION TO FRAME ID MAPPING ---------- */
        if (strcmp(argv[2], "-t") == 0)
            strcpy(frame_id, "TIT2");   // Title
        else if (strcmp(argv[2], "-a") == 0)
            strcpy(frame_id, "TPE1");   // Artist
        else if (strcmp(argv[2], "-A") == 0)
            strcpy(frame_id, "TALB");   // Album
        else if (strcmp(argv[2], "-y") == 0)
            strcpy(frame_id, "TYER");   // Year
        else if (strcmp(argv[2], "-m") == 0)
            strcpy(frame_id, "TCON");   // Genre
        else if (strcmp(argv[2], "-c") == 0)
            strcpy(frame_id, "COMM");   // Comment
        else
        {
            printf("Invalid edit option\n");
            return 1;
        }

        /* ---------- CALL EDIT FUNCTION ---------- */
        edit_tag(argv[3], frame_id, argv[4]);
    }

    /* ---------- INVALID OPTION ---------- */
    else
    {
        printf("Invalid option\n");
        printf("Use -v for view or -e for edit\n");
        return 1;
    }

    return 0;
}
