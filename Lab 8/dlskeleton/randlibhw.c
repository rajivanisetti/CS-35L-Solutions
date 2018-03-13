//
//  randlibhw.c
//  triv
//
//  Created by Rajiv Anisetti on 3/12/18.
//  Copyright © 2018 Rajiv Anisetti. All rights reserved.
//

#include "randlib.h"
#include <immintrin.h>

/* Return a random value, using hardware operations.  */
static unsigned long long
hardware_rand64 (void)
{
    unsigned long long int x;
    while (! _rdrand64_step (&x))
        continue;
    return x;
}


extern unsigned long long rand64 (void)
{
    return hardware_rand64();
}
