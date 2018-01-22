/* Copyright (c) 2018, David Hauweele <david@hauweele.net>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <err.h>

#include "safe-call.h"
#include "verbose.h"
#include "version.h"
#include "iobuf.h"
#include "load.h"

#define MAX_GEOM_STRING 32

void output(const struct pbm_img *img)
{
  char geom_string[MAX_GEOM_STRING];
  int i, j = 0;
  unsigned int prime_size = mpz_sizeinbase(img->number, 2);
  iofile_t out = iobuf_dopen(STDOUT_FILENO);

  /* FIXME: It seems that this would likely never happen,
     but we can cause it to happen on small images.
     One fix would be to add new leading lines to the image
     until we can fit the new prime size. */
  if(img->width * img->height < prime_size)
    errx(EXIT_FAILURE, "not enough leading zeros");

  verbose("leading zeros %d\n", img->width * img->height - prime_size);

  iobuf_puts(out, "P1\n");
  iobuf_puts(out, "# CREATOR: " PACKAGE_VERSION "\n");
  iobuf_puts(out, "# URL    : " WEBSITE "\n");

  snprintf(geom_string, MAX_GEOM_STRING, "%d %d", img->width, img->height);
  iobuf_puts(out, geom_string);

  for(i = img->width * img->height - prime_size - 1; i ; i--) {
    if(j++ % img->width == 0)
      iobuf_putc('\n', out);
    iobuf_putc('0', out);
  }

  for(i = prime_size ; i >= 0 ; i--) {
    if(j++ % img->width == 0)
      iobuf_putc('\n', out);
    iobuf_putc(mpz_tstbit(img->number, i) ? '1' : '0', out);
  }
  iobuf_close(out);
}
