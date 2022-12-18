/**
 * @file execute_mkdir.c
 * @brief This file creates a directory folder in the current path
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void makeDir(int current_dir_cluster_num, char * DIRNAME, FILE *img_file)
{
    directory tempDir;
    int pass_cluster = current_dir_cluster_num;
    int i = 0;
    int clusternum;
    int free_space;
    int new_offset;

    while (1)
    {
        free_space = 0;
        i = 0;
        while (i*sizeof(tempDir) < bs.BPB_BytsPerSec)
        {

            int offset = get_first_sector_of_cluster(current_dir_cluster_num)
                         + i*sizeof(tempDir);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            //check if this is empty
            if (tempDir.name[0] == 0xE5 || tempDir.name[0] == 0x00)
            {
                free_space = 1;
                new_offset = offset;
                break;
            }

            i++;
        }

        if(free_space == 1)
            break;

        //if we make it to this point, we found no empty data entries in this cluster
        //so we check where the next cluster is.

        fseek(img_file, 0x4000 + (4*current_dir_cluster_num), SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF ||
            clusternum == 0x00000000)
        {
            free_space = 0;
            break;
        }
        else
        {
            //the cluster continues, so we go on to check
            current_dir_cluster_num = clusternum;
        }
    }

    if(free_space == 1)
    {
        //allocate it!
        directory newDir;

        strcpy(newDir.name, DIRNAME);
        newDir.attr = 0x10;

        newDir.fileSize = 0;

        int empty = find_empty_cluster(img_file);
        newDir.fstClusHI = empty / 0x100;
        newDir.fstClusLO = empty % 0x100;

        fseek(img_file, new_offset, SEEK_SET);
        fwrite(&newDir, 1, sizeof(directory), img_file);
        free_space = 1;

    }
    else //did not find space
    {
        directory addDir;
        int i = find_empty_cluster(img_file);

        int setthis = 0x0FFFFFF8;

        if(i != -1)
        {
            fseek(img_file, 0x4000 + (4*i), SEEK_SET);
            fwrite(&setthis, 1, sizeof(int), img_file);

            fseek(img_file, 0x4000 + (4*pass_cluster), SEEK_SET);
            fwrite(&i, 1, sizeof(int), img_file);

            int new_offset = get_first_sector_of_cluster(i);

            strcpy(addDir.name, DIRNAME);
            addDir.attr = 0x10;
            addDir.fileSize = 0;

            int new_empty = find_empty_cluster(img_file);
            addDir.fstClusHI = new_empty / 0x100;
            addDir.fstClusLO = new_empty % 0x100;

            fseek(img_file, new_offset, SEEK_SET);
            fwrite(&addDir, 1, sizeof(directory), img_file);
        }
    }
}
