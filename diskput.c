/* 
 * File:   diskput.c
 * Author: Siqi Li
 *
 * Created on March 19, 2015, 11:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bootsector.h"
#include "fattable.h"
#include "p3utils.h"
#include "rootdir.h"

int main(int argc, char **argv) {
    FILE *fp, *inf;
    int infsize, nextRdEntry, nextFtEntry;
    char infname[128];
    if (argc < 3) {
        fprintf(stderr, "usage: diskput [IMA filename] [filename]\n");
        exit(EXIT_SUCCESS);
    }
    if ((fp = fopen(argv[1], "rb+")) == NULL) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    if ((inf = fopen(argv[2], "rb")) == NULL) {
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }
    strcpy(infname, argv[2]);
    p3_s_to_upper(infname);
    p3_s_trim(infname);
    /* Check free space */
    infsize = p3_fsize(inf);
    if (infsize > ft_get_num_of_free_sectors(fp)
            * bs_get_num_of_bytes_per_sector(fp)) {
        puts("Not enough free space in the disk image.");
        exit(EXIT_SUCCESS);
    }
    /* Find empty entry in FAT table */
    nextFtEntry = ft_find_empty_entry(fp);
    /* Find empty entry in root dir and write the file name, size, time */
    nextRdEntry = rd_find_empty_entry(fp);
    rd_write_entry(fp, nextRdEntry, infname, &nextFtEntry, &infsize);
    /* Write the file */
    rd_write_file(fp, nextFtEntry, infsize, inf);
    fclose(fp);
    fclose(inf);
    return (EXIT_SUCCESS);
}
