#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int sizeOf(int current_dir_cluster_num, char * DIRNAME, FILE* img_file)
{
    /**
     * @brief Iterates to all files and fetches the file size if the file exists.
     * 
     */
    directory tempDir;
    int i = 0;
    int clusternum;

    while (1)
    {
        i = 0;
        while (i*sizeof(tempDir) < bs.BPB_BytsPerSec)
        {
            int offset = get_first_sector_of_cluster(current_dir_cluster_num)
                         + i*sizeof(tempDir);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            if (strstr((char *)tempDir.name, DIRNAME) != NULL)
            {
                return tempDir.fileSize;
            }
            i++;
        }

        fseek(img_file, 0x4000 + (4*current_dir_cluster_num), SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8
            || clusternum == 0x0FFFFFFF
            || clusternum == 0x00000000)
        {
            printf("Error: Not a valid filename\n");
            break;
        }
        else
        {
            current_dir_cluster_num = clusternum;
        }
    }
    return 0;
}