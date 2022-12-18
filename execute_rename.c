#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int renameFile(int current_dir_cluster_num, char *DIRNAME, char *new_name, FILE *img_file) {  
    int i = 0;
    long clusternum;
    long firstCluster;
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

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF)
        {
            printf("Error: Not a file\n");
            break;
        }
        else
        {
            // int k = 0;
            // for(k=0; k<11; k++){
            //     tempDir.name[k] = "\0";
            // }
            // strcpy(tempDir.name, new_name);
            printf("%s\n", tempDir.name);
            break;
        }
    }
    return 0;
}