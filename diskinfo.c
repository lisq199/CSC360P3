/* 
 * File:   diskinfo.c
 * Author: Siqi Li
 *
 * Created on March 14, 2015, 1:47 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "bootsector.h"
#include "fattable.h"
#include "rootdir.h"

int main(int argc, char** argv) {
    FILE *fp;
    char tmp[128];
    int numOfBytesPerSector;
    if (argc < 2) {
        fprintf(stderr, "usage: diskinfo [filename]\n");
        return (EXIT_SUCCESS);
    }
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return (EXIT_FAILURE);
    }
    bs_get_oem_name(fp,tmp);
    printf("OS Name: %s\n", tmp);
    bs_get_volume_label(fp, tmp);
    printf("Label of the disk: %s\n", tmp);
    numOfBytesPerSector = bs_get_num_of_bytes_per_sector(fp);
    printf("Total size of the disk: %d bytes\n", bs_get_num_of_sectors(fp)
            * numOfBytesPerSector);
    printf("Free size of the disk: %d bytes\n", ft_get_num_of_free_sectors(fp)
            * numOfBytesPerSector);
    puts("\n===============");
    printf("The number of files in the root directory (not including "
            "subdirectories): %d\n", rd_get_num_of_files(fp));
    puts("\n===============");
    printf("Number of FAT copies: %d\n", bs_get_num_of_fat_copies(fp));
    printf("Sectors per FAT: %d\n", bs_get_num_of_sectors_per_fat(fp));
    fclose(fp);
    return (EXIT_SUCCESS);
}

