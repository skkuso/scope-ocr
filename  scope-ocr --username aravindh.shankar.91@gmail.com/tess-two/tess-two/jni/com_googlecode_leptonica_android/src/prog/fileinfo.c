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
 * fileinfo.c
 *
 *   Returns information about the image data file
 */

#include "allheaders.h"

LEPT_DLL extern const char *ImageFileFormatExtensions[];

main(int    argc,
     char **argv)
{
char        *text;
l_int32      w, h, d, wpl, count, npages, color, format, bps, spp, iscmap;
FILE        *fp;
PIX         *pix;
PIXCMAP     *cmap;
char        *filein;
static char  mainName[] = "fileinfo";

    if (argc != 2)
	exit(ERROR_INT(" Syntax:  fileinfo filein", mainName, 1));
    filein = argv[1];

    l_pngSetStrip16To8(0);  /* to preserve 16 bpp if format is png */

        /* Read the full image */
    if ((pix = pixRead(filein)) == NULL)
	exit(ERROR_INT("image not returned from file", mainName, 1));

    format = pixGetInputFormat(pix);
    pixGetDimensions(pix, &w, &h, &d);
    wpl = pixGetWpl(pix);
    fprintf(stderr, "Reading the full image:\n");
    fprintf(stderr, "  Input image format type: %s\n",
            ImageFileFormatExtensions[format]);
    fprintf(stderr, "  w = %d, h = %d, d = %d, wpl = %d\n", w, h, d, wpl);
    fprintf(stderr, "  xres = %d, yres = %d\n",
            pixGetXRes(pix), pixGetYRes(pix));

    text = pixGetText(pix);
    if (text)  /*  not null */
        fprintf(stderr, "  Text: %s\n", text);

    cmap = pixGetColormap(pix);
    if (cmap) {
        pixcmapHasColor(cmap, &color);
        if (color)
            fprintf(stderr, "  Colormap exists and has color values:");
        else
            fprintf(stderr, "  Colormap exists and has only gray values:");
	pixcmapWriteStream(stderr, pixGetColormap(pix));
    }
    else
	fprintf(stderr, "  Colormap does not exist.\n");

    if (format == IFF_TIFF || format == IFF_TIFF_G4 ||
        format == IFF_TIFF_G3 || format == IFF_TIFF_PACKBITS) {
        fprintf(stderr, "  Tiff header information:\n");
        fp = lept_fopen(filein, "rb");
        tiffGetCount(fp, &npages);
        lept_fclose(fp);
        if (npages == 1)
            fprintf(stderr, "    One page in file\n");
        else
            fprintf(stderr, "    %d pages in file\n", npages);
        fprintTiffInfo(stderr, filein);
    }

    if (d == 1) {
        pixCountPixels(pix, &count, NULL);
	fprintf(stderr, "  1 bpp: pixel ratio ON/OFF = %6.3f\n",
          (l_float32)count / (l_float32)(pixGetWidth(pix) * pixGetHeight(pix)));
    }

    pixDestroy(&pix);

        /* Test pixReadHeader() */
    if (pixReadHeader(filein, &format, &w, &h, &bps, &spp, &iscmap)) {
        fprintf(stderr, "Failure to read header!\n");
        return 1;
    }
    fprintf(stderr, "Reading just the header:\n");
    fprintf(stderr, "  Input image format type: %s\n",
            ImageFileFormatExtensions[format]);
    fprintf(stderr,
            "  w = %d, h = %d, d = %d, bps = %d, spp = %d, iscmap = %d\n",
            w, h, d, bps, spp, iscmap);
    return 0;
}

