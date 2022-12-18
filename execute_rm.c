#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int rm(int current_dir_cluster_num, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief Method to remove the file from the fat image
     * 
     */
    int i = 0;
    int clusternum = -1;
    int firstCluster;
    int save_cluster_num = current_dir_cluster_num;
    directory tempDir;

    while (1)
    {
        i = 0;
        while (i * sizeof(directory) < bs.BPB_BytsPerSec)
        {
            int offset = get_first_sector_of_cluster(current_dir_cluster_num) + i * sizeof(directory);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            if (strstr((char *)tempDir.name, DIRNAME) != NULL)
            {
                if (tempDir.attr == 0x10)
                {
                    printf("Cannot remove a directory\n");
                    return 0;
                }
                else
                {
                    firstCluster = (tempDir.fstClusHI * 0x100) + tempDir.fstClusLO;
                    // printf("herte %s", tempDir.name);
                    break;
                }
            }
            i++;
        }
        fseek(img_file, get_first_sector_of_cluster(firstCluster) + 0x40, SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF)
        {
            printf("Error: Not a file\n");
            break;
        }
        else
        {
            printf("%s\n", tempDir.name);
            current_dir_cluster_num = clusternum;
            tempDir = zeroOutDir();

            fseek(img_file, get_first_sector_of_cluster(firstCluster), SEEK_SET);
            fwrite(&tempDir, 1, sizeof(img_file), img_file);

            int x = 0x00000000;
            int offset = 0x1E0;

            fseek(img_file, get_first_sector_of_cluster(save_cluster_num) + offset, SEEK_SET);
            fwrite(&x, 1, sizeof(int), img_file);
            break;
        }
    }
    return 0;
}
