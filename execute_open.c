#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int openFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief Method to open fil, the method checks for the filename if the
     * name is directory it returns an error else if the file exists it opens and 
     * tracks it using linked list.
     * 
     */

    int i = 0;
    long clusternum;
    long firstCluster;
    directory tempDir;

    while (1)
    {
        i = 0;
        while (i * sizeof(directory) < bs.BPB_BytsPerSec)
        {
            int offset = get_first_sector_of_cluster(current_dir_cluster_num) +
                         i * sizeof(directory);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            if (strstr((char *)tempDir.name, DIRNAME) != NULL)
            {
                if (tempDir.attr == 0x10)
                {
                    printf("Cannot read a directory\n");
                    return 0;
                }
                else
                {
                    firstCluster = (tempDir.fstClusHI * 0x100) + tempDir.fstClusLO;
                    break;
                }
            }
            i++;
        }

        fseek(img_file, get_first_sector_of_cluster(firstCluster) + 0x40, SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == MAX_LIMIT1 ||
            clusternum == MAX_LIMIT2)
        {
            printf("Error: Not a file\n");
            break;
        }
        else
        {
            insert_node(DIRNAME);
            break;
        }
    }
    return 0;
}