/* 
 * File:   p3utils.c
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 3:22 PM
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Get the size of a file. 
 * Note: the file has to be opened in byte mode
 * @param fp
 * @return 
 */
long p3_fsize(FILE *fp) {
    int size, prev = ftell(fp);
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, prev, SEEK_SET);
    return size;
}

/**
 * Convert a whole string to upper case
 * @param s
 */
void p3_s_to_upper(char *s) {
    int i = -1;
    while (s[++i]) {
        s[i] = toupper(s[i]);
    }
}

void p3_s_trim(char *s) {
    char * p = s;
    int l = strlen(p);
    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;
    memmove(s, p, l + 1);
}

void p3_s_init(char *s, int len) {
    int i;
    for (i = 0; i < len; i++) {
        s[i] = '\0';
    }
}

/**
 * Get the 16-bit DOS time
 * @return 
 */
int p3_get_dos_time() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return (tm.tm_hour << 11) + (tm.tm_min << 5) + (tm.tm_sec / 2);
}

/**
 * Get the 16-bit DOS date
 * @return 
 */
int p3_get_dos_date() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return ((tm.tm_year + 1900 - 1980) << 9) + ((tm.tm_mon + 1) << 5)
            + (tm.tm_mday);
}

/**
 * Read an int from the disk image
 * @param fp
 * @param offset
 * @param len
 * @return 
 */
int p3_read_int(FILE *fp, int offset, int len) {
    int i, retVal = 0;
    int *tmp = malloc(sizeof(int));
    fseek(fp, offset, SEEK_SET);
    for (i = 0; i < len; i++) {
        *tmp = 0;
        fread(tmp, 1, 1, fp);
        retVal += *tmp << (i * 8);
    }
    free(tmp);
    return retVal;
}

/**
 * Read a string from the disk image
 * @param fp
 * @param dest
 * @param offset
 * @param len
 */
void p3_read_string(FILE *fp, char *dest, int offset, int len) {
    char *temp = malloc(512);;
    fseek(fp, offset, SEEK_SET);
    fread(temp, 1, len, fp);
    temp[len] = '\0';
    strcpy(dest, temp);
    free(temp);
}

/**
 * Read a string from the disk image and skip all the padding
 * @param fp
 * @param dest
 * @param offset
 * @param len
 */
void p3_read_string_skip_padding(FILE *fp, char *dest, int offset, int len) {
    int i;
    char c;
    fseek(fp, offset, SEEK_SET);
    for (i = 0; i < len; i++) {
        c = 0;
        fread(&c, 1, 1, fp);
        if (c == 0x20) {
            break;
        }
        dest[i] = c;
    }
    dest[i] = '\0';
}

/**
 * Get the nth byte from an int
 * @param number
 * @param n
 * @return 
 */
int p3_get_nth_byte(int number, int n) {
    return (number >> (n * 8)) & 0xFF;
}

/**
 * Write an int as little endian bytes to a file
 * @param fp
 * @param x
 * @param offset
 * @param len
 */
void p3_write_int(FILE *fp, int x, int offset, int len) {
    int i;
    fseek(fp, offset, SEEK_SET);
    for (i = 0; i < len; i++) {
        fputc(p3_get_nth_byte(x, i), fp);
    }
}

/**
 * Write a string as bytes to a file
 * @param fp
 * @param s
 * @param offset
 * @param len
 * @param pad
 */
void p3_write_string(FILE *fp, char *s, int offset, int len, char pad) {
    int i, slen = strlen(s);
    fseek(fp, offset, SEEK_SET);
    for (i = 0; i < len; i++) {
        fputc((i < slen) ? s[i] : pad, fp);
    }
}

/**
 * Get the filename without extension from a string
 * @param dest
 * @param filename
 */
void p3_get_filename(char *dest, char *filename) {
    int i, len = strlen(filename);
    for (i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            strncpy(dest, filename, i);
            return;
        }
    }
}

/**
 * Get the extension from a filename as a string
 * @param dest
 * @param filename
 */
void p3_get_extension(char *dest, char *filename) {
    int i, len = strlen(filename);
    for (i = len - 1; i >= 0; i--) {
        if (filename[i] == '.') {
            if (i == len - 1) {
                return;
            }
            strncpy(dest, filename + i + 1, 3);
        }
    }
}
