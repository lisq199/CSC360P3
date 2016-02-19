/* 
 * File:   rootdir.c
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 4:09 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p3utils.h"
#include "fattable.h"
#include "bootsector.h"

static const int base = 0x2600, offset = 32;
static const char pad = 0x20;

/**
 * Get the final file name by a file's name and extension
 * @param dest
 * @param name
 * @param extension
 */
void rd_cons_filename(char *dest, char *name, char *extension) {
    strcpy(dest, name);
    if (strlen(extension)) {
        if (strlen(extension) > 3) {
            extension[3] = '\0';
        }
        strcat(dest, ".");
        strcat(dest, extension);
    }
}

/**
 * Get the total number of files in the root dir
 * @param fp
 * @return 
 */
int rd_get_num_of_files(FILE *fp) {
    int cur = base, counter = 0;
    int *tmp = malloc(sizeof(int));
    int *attr = malloc(sizeof(int));
    fseek(fp, base, SEEK_SET);
    *tmp = 0;
    fread(tmp, 1, 1, fp);
    while (*tmp) {
        if (*tmp != 0xE5) {
            fseek(fp, cur + 11, SEEK_SET);
            *attr = 0;
            fread(attr, 1, 1, fp);
            if (*attr != 0x0F && *attr != 0x08 && *attr != 0x10) {
                counter++;
            }
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
        *tmp = 0;
        fread(tmp, 1, 1, fp);
    }
    free(tmp);
    free(attr);
    return counter;
}

/**
 * Get the volume label from root dir
 * @param fp
 * @param label
 */
void rd_get_volume_label(FILE *fp, char *label) {
    int cur = base;
    int *tmp = malloc(sizeof(int));
    int *attr = malloc(sizeof(int));
    fseek(fp, base, SEEK_SET);
    *tmp = 0;
    fread(tmp, 1, 1, fp);
    while (*tmp) {
        if (*tmp != 0xE5) {
            fseek(fp, cur + 11, SEEK_SET);
            *attr = 0;
            fread(attr, 1, 1, fp);
            if (*attr == 0x08) {
                p3_read_string_skip_padding(fp, label, cur, 11);
            }
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
        *tmp = 0;
        fread(tmp, 1, 1, fp);
    }
    free(tmp);
    free(attr);
}

/**
 * Print a list of files in the disk image
 * @param fp
 */
void rd_print_file_list(FILE *fp) {
    int cur = base;
    int *tmp = malloc(sizeof(int));
    int *attr = malloc(sizeof(int));
    char fOrD;
    int filesize;
    char *filename = malloc(9);
    char *extension = malloc(4);
    char *fullname = malloc(9 + 4 + 1);
    int date, time, year, month, day, hour, min;
    p3_s_init(filename, 9);
    p3_s_init(extension, 4);
    p3_s_init(fullname, 9 + 4 + 1);
    fseek(fp, base, SEEK_SET);
    *tmp = 0;
    fread(tmp, 1, 1, fp);
    while (*tmp) {
        if (*tmp != 0xE5) {
            /* file or directory */
            fseek(fp, cur + 11, SEEK_SET);
            *attr = 0;
            fread(attr, 1, 1, fp);
            if (*attr != 0x0F && *attr != 0x08) {
                fOrD = (*attr == 0x10) ? 'D' : 'F';
                /* size */
                filesize = p3_read_int(fp, cur + 28, 4);
                /* name */
                p3_read_string_skip_padding(fp, filename, cur, 8);
                p3_read_string_skip_padding(fp, extension, cur + 8, 3);
                rd_cons_filename(fullname, filename, extension);
                /* date and time */
                time = p3_read_int(fp, cur + 14, 2);
                date = p3_read_int(fp, cur + 16, 2);
                year = (date >> 9) + 1980;
                month = (date & 0x1FF) >> 5;
                day =  date & 0x1F;
                hour = time >> 11;
                min = (time & 0x7FF) >> 5;
                printf("%c %10d %20s %d-%02d-%02d %02d:%02d\n",
                        fOrD, filesize, fullname,
                        year, month, day, hour, min);
            }
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
        *tmp = 0;
        fread(tmp, 1, 1, fp);
    }
    free(tmp);
    free(attr);
    free(filename);
    free(extension);
    free(fullname);
}

/**
 * Get information of a file by its name
 * @param fp
 * @param name
 * @param size
 * @param cluster
 * @return 
 */
int rd_get_file_info_by_name(FILE *fp, char *name, int *size, int *cluster) {
    int cur = base;
    int *tmp = malloc(sizeof(int));
    int *attr = malloc(sizeof(int));
    char *filename = malloc(9);
    char *extension = malloc(4);
    char *fullname = malloc(9 + 4 + 1);
    char *namecopy = malloc(strlen(name) + 1);
    p3_s_init(filename, 9);
    p3_s_init(extension, 4);
    p3_s_init(fullname, 9 + 4 + 1);
    p3_s_init(namecopy, strlen(name) + 1);
    strcpy(namecopy, name);
    p3_s_to_upper(namecopy);
    p3_s_trim(namecopy);
    fseek(fp, base, SEEK_SET);
    *tmp = 0;
    fread(tmp, 1, 1, fp);
    while (*tmp) {
        if (*tmp != 0xE5) {
            fseek(fp, cur + 11, SEEK_SET);
            *attr = 0;
            fread(attr, 1, 1, fp);
            if (*attr != 0x0F && *attr != 0x08 && *attr != 0x10) {
                p3_read_string_skip_padding(fp, filename, cur, 8);
                p3_read_string_skip_padding(fp, extension, cur + 8, 3);
                rd_cons_filename(fullname, filename, extension);
                if (strcmp(fullname, namecopy) == 0) {
                    if (size != NULL) {
                        *size = p3_read_int(fp, cur + 28, 4);
                    }
                    if (cluster != NULL) {
                        *cluster = p3_read_int(fp, cur + 26, 2);
                    }
                    return 1;
                }
            }
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
        *tmp = 0;
        fread(tmp, 1, 1, fp);
    }
    free(tmp);
    free(attr);
    free(filename);
    free(extension);
    free(fullname);
    free(namecopy);
    return 0;
}

/**
 * Read a file from the disk image and put its content into another file
 * @param fp the input file
 * @param firstCluster the first cluster in FAT table
 * @param size the size of the file
 * @param outf the output file
 */
void rd_read_file(FILE *fp, int firstCluster, int size, FILE *outf) {
    int i, cur = firstCluster, sectorsRead = 0, numOfBytesToRead;
    const int bytesPerSector = bs_get_num_of_bytes_per_sector(fp),
            totalNumOfSectors = size / bytesPerSector,
            remainder = size % bytesPerSector;
    int *tmp = malloc(sizeof(int));
    long totalBytesRead = 0;
    while (!(cur >= 0xFF8 && cur <= 0xFFF)) {
        sectorsRead++;
        numOfBytesToRead = (sectorsRead <= totalNumOfSectors)
                ? bytesPerSector
                : remainder;
        fseek(fp, (cur + 31) * bytesPerSector, SEEK_SET);
        for (i = 0; i < numOfBytesToRead; i++) {
            *tmp = 0;
            fread(tmp, 1, 1, fp);
            fputc(*tmp, outf);
            if (++totalBytesRead >= size) {
                break;
            }
        }
        cur = ft_read_entry(fp, cur);
    }
    free(tmp);
}

/**
 * Find the next empty entry in the root dir
 * @param fp
 * @return the index of the next empty entry
 */
int rd_find_empty_entry(FILE *fp) {
    int cur = base, counter = 0;
    int *tmp = malloc(sizeof(int));
    fseek(fp, base, SEEK_SET);
    *tmp = 0;
    fread(tmp, 1, 1, fp);
    while (*tmp) {
        if (*tmp == 0xE5) {
            return counter;
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
        *tmp = 0;
        fread(tmp, 1, 1, fp);
        counter++;
    }
    free(tmp);
    return counter;
}

void rd_write_entry(FILE *fp, int index, char *filename,
        int *firstCluster, int *size) {
    int cur = base, time, date;
    char *tmpStr = malloc(10);
    cur += offset * index;
    p3_s_init(tmpStr, 10);
    if (filename != NULL) {
        p3_get_filename(tmpStr, filename);
        p3_write_string(fp, tmpStr, cur, 8, pad);
        p3_get_extension(tmpStr, filename);
        p3_write_string(fp, tmpStr, cur + 8, 3, pad);
    }
    if (firstCluster != NULL) {
        p3_write_int(fp, *firstCluster, cur + 26, 2);
    }
    if (size != NULL) {
        p3_write_int(fp, *size, cur + 28, 4);
    }
    time = p3_get_dos_time();
    date = p3_get_dos_date();
    p3_write_int(fp, time, cur + 14, 2);
    p3_write_int(fp, date, cur + 16, 2);
    p3_write_int(fp, time, cur + 22, 2);
    p3_write_int(fp, date, cur + 24, 2);
    free(tmpStr);
}

void rd_write_file(FILE *fp, int firstCluster, int size, FILE *inf) {
    int i, cur = firstCluster, next, finished = 0, bytesRead = 0;
    const int bytesPerSector = bs_get_num_of_bytes_per_sector(fp);
    int *tmp = malloc(sizeof(int));
    fseek(inf, 0, SEEK_SET);
    while (!finished) {
        fseek(fp, (cur + 31) * bytesPerSector, SEEK_SET);
        for (i = 0; i < bytesPerSector; i++) {
            *tmp = 0;
            if (fread(tmp, 1, 1, inf)) {
                bytesRead++;
                fputc(*tmp, fp);
            } else {
                finished = 1;
            }
        }
        ft_write_entry(fp, cur, 1);
        next = ft_find_empty_entry(fp);
        ft_write_entry(fp, cur, next);
        cur = next;
    }
    ft_write_entry(fp, cur, 0xFFF);
    free(tmp);
}
