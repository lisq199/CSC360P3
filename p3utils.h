/* 
 * File:   p3utils.h
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 3:23 PM
 */

#ifndef P3UTILS_H
#define	P3UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

    int p3_fsize(FILE *fp);
    void p3_s_to_upper(char *s);
    void p3_s_trim(char *s);
    void p3_s_init(char *s, int len);
    int p3_get_dos_time();
    int p3_get_dos_date();
    int p3_read_int(FILE *fp, int offset, int len);
    void p3_read_string(FILE *fp, char *dest, int offset, int len);
    void p3_read_string_skip_padding(FILE *fp, char *dest, int offset, int len);
    int p3_get_nth_byte(int number, int n);
    void p3_write_int(FILE *fp, int x, int offset, int len);
    void p3_write_string(FILE *fp, char *s, int offset, int len, char pad);
    void p3_get_filename(char *dest, char *filename);
    void p3_get_extension(char *dest, char *filename);

#ifdef	__cplusplus
}
#endif

#endif	/* P3UTILS_H */

