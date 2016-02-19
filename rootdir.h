/* 
 * File:   rootdir.h
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 4:10 PM
 */

#ifndef ROOTDIR_H
#define	ROOTDIR_H

#ifdef	__cplusplus
extern "C" {
#endif

    void rd_cons_filename(char *dest, char *name, char *extension);
    int rd_get_num_of_files(FILE *fp);
    void rd_get_volume_label(FILE *fp, char *label);
    void rd_print_file_list(FILE *fp);
    int rd_get_file_info_by_name(FILE *fp, char *name, int *size, int *cluster);
    void rd_read_file(FILE *fp, int firstCluster, int size, FILE *outf);
    int rd_find_empty_entry(FILE *fp);
    void rd_write_entry(FILE *fp, int index, char *filename,
            int *firstCluster, int *size);
    void rd_write_file(FILE *fp, int firstCluster, int size, FILE *inf);


#ifdef	__cplusplus
}
#endif

#endif	/* ROOTDIR_H */

