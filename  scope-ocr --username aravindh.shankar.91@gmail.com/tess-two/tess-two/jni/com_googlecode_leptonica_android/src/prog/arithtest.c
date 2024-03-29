/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

/*
 * arithtest.c
 *
 */

#include "allheaders.h"

main(int    argc,
     char **argv)
{
char        *filein;
l_int32      w, h, same;
PIX         *pixs, *pix1, *pix2, *pix3, *pix4, *pix5;
static char  mainName[] = "arithtest";

    if (argc != 2)
	exit(ERROR_INT(" Syntax:  arithtest filein", mainName, 1));

    filein = argv[1];

    if ((pixs = pixRead(filein)) == NULL)
	exit(ERROR_INT("pix not made", mainName, 1));

    w = pixGetWidth(pixs);
    h = pixGetHeight(pixs);

        /* input a grayscale image and convert it to 16 bpp */
    pix1 = pixInitAccumulate(w, h, 0);
    pixAccumulate(pix1, pixs, L_ARITH_ADD);
    pixMultConstAccumulate(pix1, 255., 0);
    pix2 = pixFinalAccumulate(pix1, 0, 16);
    l_pngSetStrip16To8(0);
    pixWrite("/tmp/junkpix1.png", pix2, IFF_PNG);

        /* convert it back to 8 bpp, linear mapped */
    pix3 = pixMaxDynamicRange(pix2, L_LINEAR_SCALE);
    pixWrite("/tmp/junkpix2.png", pix3, IFF_PNG);

        /* convert it back to 8 bpp using the MSB */
    pix4 = pixRead("/tmp/junkpix1.png");
    pix5 = pixConvert16To8(pix4, 1);
    pixWrite("/tmp/junkpix3.png", pix5, IFF_PNG);
    return 0;
}

