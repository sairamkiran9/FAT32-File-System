/**
 * @file execute_creat.c
 * @brief This file has method to find the empty allocation in the fat img
 * and another method that generates a file based on the space avaliability.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int find_empty_cluster(FILE *img_file)
{
    /**
     * @brief check for the cluster that is free to allocate new file
     *
     */
    int i;
    int first_data_sector = bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs + bs.BPB_FATSz32);
    for (i = 0; 0x4000 + (4 * i) < first_data_sector * bs.BPB_BytsPerSec; i++)
    {
        int clusternum;
        fseek(img_file, 0x4000 + (4 * i), SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x00000000)
        {
            return i;
        }
    }
    return -1;
}

void createFile(int current_dir_cluster_num, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief creates the file in the current directory
     *
     */
    directory tempDir;
    int pass_cluster = current_dir_cluster_num;

    int i = 0;
    int clusternum;
    int free_space;
    int new_offset;

    while (1)
    {
        i = 0;
        free_space = 0;
        while (i * sizeof(tempDir) < bs.BPB_BytsPerSec)
        {

            int offset = get_first_sector_of_cluster(current_dir_cluster_num) +
                         i * sizeof(tempDir);
            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            if (tempDir.name[0] == 0xE5 || tempDir.name[0] == 0x00)
            {

                free_space = 1;
                new_offset = offset;
                break;
            }
            i++;
        }

        if (free_space == 1)
            break;

        fseek(img_file, 0x4000 + (4 * current_dir_cluster_num), SEEK_SET);
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
            current_dir_cluster_num = clusternum;
        }
    }

    if (free_space == 1)
    {
        directory newDir;

        strcpy(newDir.name, DIRNAME);
        newDir.attr = 0;
        newDir.fileSize = 0;

        int empty = find_empty_cluster(img_file);
        newDir.fstClusLO = empty % 0x100;
        newDir.fstClusHI = empty / 0x100;

        fseek(img_file, new_offset, SEEK_SET);
        fwrite(&newDir, 1, sizeof(directory), img_file);
        free_space = 1;
    }
    else
    {
        directory addDir;
        int i = find_empty_cluster(img_file);

        int flag = 0x0FFFFFF8;
        if (i != -1)
        {
            fseek(img_file, 0x4000 + (4 * i), SEEK_SET);
            fwrite(&flag, 1, sizeof(int), img_file);

            fseek(img_file, 0x4000 + (4 * pass_cluster), SEEK_SET);
            fwrite(&i, 1, sizeof(int), img_file);

            int new_offset = get_first_sector_of_cluster(i);

            strcpy(addDir.name, DIRNAME);
            addDir.attr = 0;
            addDir.fileSize = 0;

            int new_empty = find_empty_cluster(img_file);
            addDir.fstClusLO = new_empty % 0x100;
            addDir.fstClusHI = new_empty / 0x100;
            

            fseek(img_file, new_offset, SEEK_SET);
            fwrite(&addDir, 1, sizeof(directory), img_file);
        }
    }
}