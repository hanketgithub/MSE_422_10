//
//  mse.c
//  mse
//
//  Created by Hank Lee on 12/21/15.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//

#include <stdint.h>
#include <stdio.h>
#include <math.h>

int mse
(
        uint32_t  wxh,
        double   *mse_luma,
        double   *mse_cr,
        double   *mse_cb,
  const uint16_t *src,
  const uint16_t *cmp
)
{
    int i;
    double sum_luma, sum_cr, sum_cb;
    
    sum_luma    = 0;
    sum_cr      = 0;
    sum_cb      = 0;
    
    // MSE Luma
    for (i = 0; i < wxh; i++)
    {
        int x = src[i];
        int y = cmp[i];
        
        //printf("diff[%d]=%d\n", i, y - x);
        
        sum_luma += pow((y - x), 2);
    }

	//printf("sum_luma=%f\n", sum_luma);
    
    // MSE Cr
    const uint16_t *cr_src = &src[wxh];
    const uint16_t *cr_cmp = &cmp[wxh];
    for (i = 0; i < wxh / 2; i++)
    {
        int x = cr_src[i];
        int y = cr_cmp[i];
        
        sum_cr += pow((y - x), 2);
    }

	//printf("sum_cr=%f\n", sum_cr);

    // MSE Cb
    const uint16_t *cb_src = &src[wxh + wxh / 2];
    const uint16_t *cb_cmp = &cmp[wxh + wxh / 2];
    for (i = 0; i < wxh / 2; i++)
    {
        int x = cb_src[i];
        int y = cb_cmp[i];
        
        sum_cb += pow((y - x), 2);
    }
    
    //printf("sum_luma=%f\n", sum_luma);
    //printf("sum_cr=%f\n", sum_cr);
    //printf("sum_cb=%f\n", sum_cb);
    
    *mse_luma   = sum_luma / wxh;
    *mse_cr     = sum_cr / (wxh / 2);
    *mse_cb     = sum_cb / (wxh / 2);
    
    return 0;
}
