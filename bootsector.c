/* 
 * File:   bootsector.c
 * Author: Siqi Li
 *
 * Created on March 14, 2015, 2:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p3utils.h"
#include "rootdir.h"

void bs_get_oem_name(FILE *fp, char *osname) {
    p3_read_string(fp, osname, 3, 8);
}

int bs_get_num_of_bytes_per_sector(FILE *fp) {
    return p3_read_int(fp, 11, 2);
}

int bs_get_num_of_sectors_per_cluster(FILE *fp) {
    return p3_read_int(fp, 13, 1);
}

int bs_get_num_of_reserved_sectors(FILE *fp) {
    return p3_read_int(fp, 14, 2);
}

int bs_get_num_of_fat_copies(FILE *fp) {
    return p3_read_int(fp, 16, 1);
}

int bs_get_num_of_root_entries(FILE *fp) {
    return p3_read_int(fp, 17, 2);
}

int bs_get_num_of_sectors(FILE *fp) {
    return p3_read_int(fp, 19, 2);
}

int bs_get_num_of_sectors_per_fat(FILE *fp) {
    return p3_read_int(fp, 22, 2);
}

/**
 * Get the volume label in the boot sector. If it's not available, then 
 * get it from the root dir
 * @param fp
 * @param label
 */
void bs_get_volume_label(FILE *fp, char *label) {
    p3_read_string_skip_padding(fp, label, 43, 11);
    if (!strlen(label)) {
        rd_get_volume_label(fp, label);
    }
}
