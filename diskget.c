/* 
 * File:   diskget.c
 * Author: Siqi Li
 *
 * Created on March 19, 2015, 6:14 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "rootdir.h"

int main(int argc, char **argv) {
    FILE *fp, *outf;
    int size, cluster;
    if (argc < 3) {
        fprintf(stderr, "usage: diskget [IMA filename] [filename]\n");
        return (EXIT_SUCCESS);
    }
    if ((fp = fopen(argv[1], "rb")) == NULL) {
        perror(argv[1]);
        return (EXIT_FAILURE);
    }
    if ((outf = fopen(argv[2], "wb")) == NULL) {
        perror(argv[1]);
        return (EXIT_FAILURE);
    }
    if (!rd_get_file_info_by_name(fp, argv[2], &size, &cluster)) {
        puts("File not found.");
        return (EXIT_SUCCESS);
    }
    rd_read_file(fp, cluster, size, outf);
    fclose(fp);
    fclose(outf);
    return (EXIT_SUCCESS);
}
