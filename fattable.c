/* 
 * File:   fattable.c
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 3:38 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "bootsector.h"
#include "p3utils.h"
#include "fattable.h"

static const int base = 0x200;

/**
 * Read an entry in the FAT table
 * @param fp
 * @param offset the offset to the first entry of the FAT table
 * @return 
 */
int ft_read_entry(FILE *fp, int offset) {
    int *tmp1 = malloc(sizeof(int));
    int *tmp2 = malloc(sizeof(int));
    int result;
    fseek(fp, base + 3 * offset / 2, SEEK_SET);
    *tmp1 = 0;
    *tmp2 = 0;
    fread(tmp1, 1, 1, fp);
    fread(tmp2, 1, 1, fp);
    if (offset % 2 == 0) {
        *tmp2 = *tmp2 & 0x0F;
        result = (*tmp2 << 8) + *tmp1;
    } else {
        *tmp1 = *tmp1 & 0xF0;
        result = (*tmp2 << 4) + (*tmp1 >> 4);
    }
    free(tmp1);
    free(tmp2);
    return result;
}

/**
 * Overwrite a FAT table entry
 * @param fp
 * @param index
 * @param val
 */
void ft_write_entry(FILE *fp, int index, int val) {
    int *tmp1 = malloc(sizeof(int));
    int *tmp2 = malloc(sizeof(int));
    int *tmpByte = malloc(sizeof(int));
    int cur = base + 3 * index / 2;
    fseek(fp, cur, SEEK_SET);
    *tmp1 = 0;
    *tmp2 = 0;
    fread(tmp1, 1, 1, fp);
    fread(tmp2, 1, 1, fp);
    if (index % 2 == 0) {
        p3_write_int(fp, (val & 0xFF), cur, 1);
        *tmpByte = (val >> 8) & 0x0F;
        p3_write_int(fp, ((*tmp2) & 0xF0) + *tmpByte, cur + 1, 1);
    } else {
        p3_write_int(fp, (val >> 4) & 0xFF, cur + 1, 1);
        *tmpByte = (val & 0x0F) << 4;
        p3_write_int(fp, ((*tmp1) & 0x0F) + *tmpByte, cur, 1);
    }
    free(tmp1);
    free(tmp2);
    free(tmpByte);
}

int ft_get_num_of_free_sectors(FILE *fp) {
    const int num_of_sectors = bs_get_num_of_sectors(fp);
    int i, counter = 0;
    for (i = 2; i < num_of_sectors - 32; i++) {
        if (!ft_read_entry(fp, i)) {
            counter++;
        }
    }
    return counter;
}

int ft_find_empty_entry(FILE *fp) {
    int i;
    const int num_of_sectors = bs_get_num_of_sectors(fp);
    for (i = 2; i < num_of_sectors - 32; i++) {
        if (!ft_read_entry(fp, i)) {
            return i;
        }
    }
    return -1;
}
