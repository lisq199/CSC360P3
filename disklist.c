/* 
 * File:   disklist.c
 * Author: Siqi Li
 *
 * Created on March 18, 2015, 4:48 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "rootdir.h"

int main(int argc, char** argv) {
    FILE *fp;
    if (argc < 2) {
        fprintf(stderr, "usage: disklist [filename]\n");
        return (EXIT_SUCCESS);
    }
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return (EXIT_FAILURE);
    }
    rd_print_file_list(fp);
    fclose(fp);
    return (EXIT_SUCCESS);
}
