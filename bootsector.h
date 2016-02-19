/* 
 * File:   bootsector.h
 * Author: Siqi Li
 *
 * Created on March 14, 2015, 2:22 PM
 */

#ifndef BOOTSECTOR_H
#define	BOOTSECTOR_H

#ifdef	__cplusplus
extern "C" {
#endif

    void bs_get_oem_name(FILE *fp, char *osname);
    int bs_get_num_of_bytes_per_sector(FILE *fp);
    int bs_get_num_of_sectors_per_cluster(FILE *fp);
    int bs_get_num_of_reserved_sectors(FILE *fp);
    int bs_get_num_of_fat_copies(FILE *fp);
    int bs_get_num_of_root_entries(FILE *fp);
    int bs_get_num_of_sectors(FILE *fp);
    int bs_get_num_of_sectors_per_fat(FILE *fp);
    void bs_get_volume_label(FILE *fp, char *label);


#ifdef	__cplusplus
}
#endif

#endif	/* BOOTSECTOR_H */

