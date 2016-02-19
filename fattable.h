/* 
 * File:   fattable.h
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 3:38 PM
 */

#ifndef FATTABLE_H
#define	FATTABLE_H

#ifdef	__cplusplus
extern "C" {
#endif

    int ft_read_entry(FILE *fp, int offset);
    void ft_write_entry(FILE *fp, int index, int val);
    int ft_get_num_of_free_sectors(FILE *fp);
    int ft_find_empty_entry(FILE *fp);


#ifdef	__cplusplus
}
#endif

#endif	/* FATTABLE_H */

