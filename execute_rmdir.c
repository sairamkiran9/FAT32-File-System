#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

directory zeroOutDir()
{
    /**
     * @brief cleans the complete structure
     * 
     */
    directory tempDir;

    tempDir.LDIR_Ord = '\0';
    tempDir.LDIR_Name1[0] = 0;
    tempDir.LDIR_Attr = '\0';
    tempDir.LDIR_Type = '\0';
    tempDir.LDIR_Chksum = '\0';
    tempDir.LDIR_Name2[0] = 0;
    tempDir.LDIR_FstClusLO = 0;
    tempDir.LDIR_Name3 = 0;

    tempDir.name[0] = 0;
    tempDir.attr = '\0';
    tempDir.ntres = '\0';
    tempDir.crtTimeTenth = '\0';
    tempDir.crtTime = 0;
    tempDir.crtDate = 0;
    tempDir.lstAccDate = 0;
    tempDir.fstClusHI = 0;
    tempDir.wrtTime = 0;
    tempDir.wrtDate = 0;
    tempDir.fstClusLO = 0;
    tempDir.fileSize = 0;

    return tempDir;
}

void rmDir(int current_dir_cluster_num, char *DIRNAME, FILE *img_file)
{
    /**
     * @brief method to delete the complete directory if it is empty.
     * 
     */
    directory tempDir;

    int save_cluster_num = current_dir_cluster_num;

    int i;
    int clusternum;
    int firstCluster;
    while (1)
    {
        i = 0;
        while (i * sizeof(tempDir) < bs.BPB_BytsPerSec)
        {

            int offset = get_first_sector_of_cluster(current_dir_cluster_num) + i * sizeof(tempDir);

            fseek(img_file, offset, SEEK_SET);
            fread(&tempDir, sizeof(directory), 1, img_file);

            if (strstr((char *)tempDir.name, DIRNAME) != NULL &&
                tempDir.attr == 0x10)
            {
                firstCluster = (tempDir.fstClusHI * 0x100) + tempDir.fstClusLO;
                break;
            }

            i++;
        }

        fseek(img_file, get_first_sector_of_cluster(firstCluster) + 0x40, SEEK_SET);
        fread(&clusternum, sizeof(int), 1, img_file);

        if (clusternum == 0x0FFFFFF8 ||
            clusternum == 0x0FFFFFFF)
        {
            printf("Error: Not a directory\n");
            break;
        }
        else if (clusternum != 0x00)
        {
            printf("Error: Directory not empty\n");
            break;
        }
        else
        {
            current_dir_cluster_num = clusternum;
            tempDir = zeroOutDir();

            fseek(img_file, get_first_sector_of_cluster(firstCluster), SEEK_SET);
            fwrite(&tempDir, 2, sizeof(img_file), img_file);

            int x = 0x00000000;
            int offset = 0x1E0;

            fseek(img_file, get_first_sector_of_cluster(save_cluster_num) + offset, SEEK_SET);
            fwrite(&x, 1, sizeof(int), img_file);
            break;
        }
    }
}
