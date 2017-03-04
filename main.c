//
//  main.c
//  mse
//
//  Useage: mse [source file] [compare file] [width] [height]
//
//  Created by Hank Lee on 12/21/15.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "mse.h"
#include "psnr.h"


#define MAX_WIDTH   7680
#define MAX_HEIGHT  4320


static uint8_t img[MAX_WIDTH * MAX_HEIGHT * 2 * 2];
static uint8_t cmp[MAX_WIDTH * MAX_HEIGHT * 2 * 2];

int main(int argc, const char * argv[]) {
    int fd_src;
    int fd_cmp;
    int fd_mse;
    int fd_psnr;

    ssize_t rd_sz;
    
    uint32_t width;
    uint32_t height;
    uint32_t wxh;
    double   mse_luma;
    double   mse_cr;
    double   mse_cb;
    double   psnr_luma;
    double   psnr_cr;
    double   psnr_cb;
    
    char *cp;
    char output[256];
    
    if (argc < 5)
    {
        fprintf(stderr, "useage: %s [src_file] [cmp_file] [width] [height]\n", argv[0]);
        
        return -1;
    }
    
    
    rd_sz       = 0;
    width       = 0;
    height      = 0;
    wxh         = 0;
    mse_luma    = 0;
    mse_cr      = 0;
    mse_cb      = 0;
    psnr_luma   = 0;
    psnr_cr     = 0;
    psnr_cb     = 0;
    cp          = NULL;


    // get src input file name from comand line
    fd_src = open(argv[1], O_RDONLY);
    if (fd_src < 0)
    {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    // get cmp input file name from comand line
    fd_cmp = open(argv[2], O_RDONLY);
    if (fd_cmp < 0)
    {
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }

    // specify output file name
    cp = strrchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_mse.csv");
    
    fd_mse = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );

        
    // specify output file name
    memset(output, 0, sizeof(output));
    cp = strrchr(argv[1], '.');
    strncpy(output, argv[1], cp - argv[1]);
    strcat(output, "_psnr.csv");
    
    fd_psnr = open
            (
             output,
             O_WRONLY | O_CREAT | O_TRUNC,
             S_IRUSR
            );

    width   = atoi(argv[3]);
    height  = atoi(argv[4]);
    
    wxh = width * height;

    
    fprintf(stderr, "Processing: ");

    while (1)
    {
        rd_sz = read(fd_src, img, wxh * 2 * 2);
        rd_sz = read(fd_cmp, cmp, wxh * 2 * 2);
        
        if (rd_sz == wxh * 2 * 2)
        {            
            mse
            (
                wxh,
                &mse_luma,
                &mse_cr,
                &mse_cb,
                (uint16_t *) img,
                (uint16_t *) cmp
            );
            
            psnr_luma   = psnr(mse_luma);
            psnr_cr     = psnr(mse_cr);
            psnr_cb     = psnr(mse_cb);

            char buf0[200];
            char buf1[200];
            
            sprintf(buf0, "%f,%f,%f\n", mse_luma, mse_cr, mse_cb);
            sprintf(buf1, "%f,%f,%f\n", psnr_luma, psnr_cr, psnr_cb);
            
            write(fd_mse, buf0, strlen(buf0));
            write(fd_psnr, buf1, strlen(buf1));
        }
        else
        {
            break;
        }
        fputc('.', stderr);
        fflush(stderr);
    }

    close(fd_src);
    close(fd_cmp);
    close(fd_mse);
    close(fd_psnr);
    
    fprintf(stderr, "Done\n");
    fprintf(stderr, "Output file: %s\n", output);
    
    return 0;
}

