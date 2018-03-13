//
//  randlibsw.c
//  triv
//
//  Created by Rajiv Anisetti on 3/12/18.
//  Copyright © 2018 Rajiv Anisetti. All rights reserved.
//

#include "randlib.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/* Software implementation.  */

/* Input stream containing random bytes.  */
static FILE *urandstream;

/* Initialize the software rand64 implementation.  */

__attribute__((__constructor__))
static void
software_rand64_init (void)
{
    urandstream = fopen ("/dev/urandom", "r");
    if (! urandstream)
        abort ();
}

/* Return a random value, using software operations.  */
static unsigned long long
software_rand64 (void)
{
    unsigned long long int x;
    if (fread (&x, sizeof x, 1, urandstream) != 1)
        abort ();
    return x;
}

/* Finalize the software rand64 implementation.  */

__attribute__((__destructor__))
static void
software_rand64_fini (void)
{
    fclose (urandstream);
}

extern unsigned long long rand64 (void)
{
    return software_rand64();
}

