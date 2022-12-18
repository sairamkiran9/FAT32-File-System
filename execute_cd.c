/**
 * @file execute_cd.c
 * @brief This file contains methods for moving the directory back and
 * the method to change the directory if exists.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void move_to_prev_path(void)
{
    /**
     * @brief get back to the prev directory based on the env file list.
     * 
     */
    if (env.current > 1)
    {
        env.current_cluster_number = env.current_cluster_path[env.current-2];
        strcpy((char *)env.name, env.currentpath[env.current-2]);
        env.current--;
    }
}

int cd(int current_dir_cluster_num, char * DIRNAME, FILE* img_file)
{
    /**
     * @brief The cd method to change the directory if exists in the path.
     * 
     */
    
    int i = 0;
    int clusternum;
    int save_cluster_num = current_dir_cluster_num;
    directory tempDir;

    while (1)
    {
        i = 0;
        while (i*sizeof(tempDir) < bs.BPB_BytsPerSec)
        {
            int offset = get_first_sector_of_cluster(current_dir_cluster_num)
                         + i*sizeof(tempDir);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            int isdirectory = tempDir.attr&0x10;
            if (strstr((char *)tempDir.name, DIRNAME) != NULL  && isdirectory == 0x10)
            {
                return tempDir.fstClusHI * 0x100 + tempDir.fstClusLO;
            }
            i++;
        }

        fseek(img_file, 0x4000 + (4*current_dir_cluster_num), SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF ||
            clusternum == 0x00000000)
        {
            printf("Error: Not a directory\n");
            break;
        }
        else
        {
            current_dir_cluster_num = clusternum;
        }
    }
    return save_cluster_num;
}