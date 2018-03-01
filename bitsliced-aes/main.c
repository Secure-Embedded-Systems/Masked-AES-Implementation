

/*
 *
 * Virginia Tech
 * Secure Embedded Systems Lab
 *
 * Copyright (C) 2017 Virginia Tech
 *
 * Written in 2017 by Conor Patrick. 
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */





/*#include <stdio.h>*/
#include <stdlib.h>
#include <string.h>
#include "bs.h"
#include "aes.h"
#include "utils.h"

#ifdef RUN_TESTS
#include "tests/tests.h"
#endif

#ifdef TEST_FOOTPRINT
#define printf(fmt, ...) (0)
#define fprintf(f,fmt, ...) (0)
#else
#include <stdio.h>
#endif

int main(int argc, char * argv[])
{

#ifdef RUN_TESTS
#ifndef TEST_FOOTPRINT
    aes_ecb_test();
#endif
    aes_ctr_test();

#else

    cli_app(argc,argv);

#endif



    return 0;
}


