//
//  psnr.c
//  mse
//
//  Created by Hank Lee on 12/21/15.
//  Copyright (c) 2015 Hank Lee. All rights reserved.
//

#include <math.h>

/*!
  \def TWENTY_MULTIPLY_LOG_BASE_10_OF_MAX
  Precomputed the value of the formula: 20 * log10(1023)
 */
#define TWENTY_MULTIPLY_LOG_BASE_10_OF_MAX		(60.197512)


double psnr
(
    double mse
)
{
    return TWENTY_MULTIPLY_LOG_BASE_10_OF_MAX - 10 * log10(mse);
}

