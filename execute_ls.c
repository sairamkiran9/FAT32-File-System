/**
 * @file execute_ls.c
 * @brief This file contains the methods ls and ls DIRNAME
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void ls(int current_cluster_number, FILE *img_file)
{
    /**
     * @brief This method gets the offset from the first cluster and than fetches
     * prints all the files/folders in the current path
     *
     */

    int i = 0;
    int flag = 0;
    int clusternum;
    directory tempDir;
    
    while (1)
    {
        i = 0;
        while (i * sizeof(tempDir) < bs.BPB_BytsPerSec)
        {
            int offset = get_first_sector_of_cluster(current_cluster_number) + i * sizeof(tempDir);
            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);
            if (flag == 0)
            {
                printf(".\n");
                flag = 1;
            }
            if (strcmp((char *)tempDir.name, "") != 0)
                printf("%s\n", tempDir.name);
            i++;
        }

        fseek(img_file, 0x4000 + (4 * current_cluster_number), SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF ||
            clusternum == 0x00000000)
        {
            break;
        }
        else
        {
            current_cluster_number = clusternum;
        }
    }
}

void lsDir(int current_cluster_number, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief This method calls cd to check the directory if exists and
     * ls method is called from that directory to print the contents
     *
     */
    int cluster_returned = cd(current_cluster_number, DIRNAME, img_file); // call cd method
    if (cluster_returned != current_cluster_number)
    {
        ls(cluster_returned, img_file); // list directory (call ls method)
    }
}