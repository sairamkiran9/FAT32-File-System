#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

int readFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief File that prints all the contents of the file is exists
     * 
     */
    int i = 0;
    long clusternum = -1;
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
            int numOfBytes = tempDir.fileSize;
            printf("Reading %s of size %d\n", tempDir.name, numOfBytes);
            fseek(img_file, get_first_sector_of_cluster(firstCluster), SEEK_SET);
            char *bytes = malloc(numOfBytes);
            fread(bytes, numOfBytes, 1, img_file);
            printf("%s", bytes);
            break;
        }
    }
    return 0;
}