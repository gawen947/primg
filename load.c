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

#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <gmp.h>

#include "safe-call.h"
#include "verbose.h"
#include "common.h"
#include "iobuf.h"
#include "xatoi.h"
#include "load.h"

#define MAX_ROW_SIZE 65536
#define MAX_IMG_SIZE (4096 * 4096)

static struct pbm_img *img;

static ssize_t xiobuf_read(const iofile_t file, void *buf, size_t count)
{
  ssize_t n = iobuf_read(file, buf, count);
  if(n < 0)
    err(EXIT_FAILURE, "cannot read");
  return n;
}

static ssize_t xxiobuf_gets(const iofile_t file, char *buf, size_t count)
{
  ssize_t n = iobuf_gets(file, buf, count);
  if(n <= 0) {
    if(n < 0)
      err(EXIT_FAILURE, "cannot read line");
    else
      errx(EXIT_FAILURE, "unexpected end of file");
  }

  return n;
}

static ssize_t until_no_comment(const iofile_t file, char *buf, size_t count)
{
  ssize_t n;
  do
    n = xxiobuf_gets(file, buf, count);
  while(buf[0] == '#');

  return n;
}

static struct pbm_img * load_pbm_p1(const iofile_t file, char *buf, size_t count)
{
  int err, size;
  unsigned int w, h;
  ssize_t n;
  char *w_s, *h_s;

  n = until_no_comment(file, buf, count);
  strip_gets_newline(buf, n);

  w_s = buf;
  h_s = memchr(buf, ' ', n);


  if(!h_s)
    errx(EXIT_FAILURE, "invalid pbm header");

  *h_s = '\0';
  h_s++;

  w = xatou(w_s, &err);
  if(err != XATOI_SUCCESS)
    errx(EXIT_FAILURE, "invalid pbm width");
  h = xatou(h_s, &err);
  if(err != XATOI_SUCCESS)
    errx(EXIT_FAILURE, "invalid pbm height");

  size = w * h;
  if(size > MAX_IMG_SIZE)
    errx(EXIT_FAILURE, "image too large");

  img = xmalloc(sizeof(struct pbm_img));

  img->width  = w;
  img->height = h;

  /* We may count the leading zeros here and alloc a smaller GMP integer,
     but I'm not sure that would be worth the try (memory wise). */
  mpz_init2(img->number, size);

  while((n = xiobuf_read(file, buf, count))) {
    const char *s = buf;

    while(n--) {
      if(size == 0) {
        warnx("garbage after raster data");
        goto EXIT;
      }

      switch(*s++) {
      case ' ':
      case '\n':
        break;
      case '0':
        /* FIXME: I'm not sure we need this.
           What if GMP integer are initialized
           to zero? */
        mpz_clrbit(img->number, --size);
        break;
      case '1':
        mpz_setbit(img->number, --size);
        break;
      }
    }
  }

  if(size)
    warnx("incomplete raster data");

EXIT:
  verbose("PBM ASCII image loaded (%dx%d)\n", w, h);
  return img;
}

static struct pbm_img * load_pbm_p4(const iofile_t file, char *buf, size_t count)
{
  UNUSED(file);
  UNUSED(buf);
  UNUSED(count);

  /* TODO: please implement this :(
           it's not that hard. */
  errx(EXIT_FAILURE, "not implemented yet :(");
  return NULL;
}

struct pbm_img * load_pbm(const char *path)
{
  char row[MAX_ROW_SIZE];
  iofile_t img;
  ssize_t n;

  if(path)
    img = iobuf_open(path, O_RDONLY, 0);
  else
    img = iobuf_dopen(STDIN_FILENO);

  if(!img)
    err(EXIT_FAILURE, "cannot open pbm");

  /* check for magic */
  n = until_no_comment(img, row, MAX_ROW_SIZE);
  strip_gets_newline(row, n);

  if(!strcmp(row, "P1"))
    return load_pbm_p1(img, row, MAX_ROW_SIZE);
  else if(!strcmp(row, "P4"))
    return load_pbm_p4(img, row, MAX_ROW_SIZE);
  else
    errx(EXIT_FAILURE, "invalid pbm magic");
  return 0; /* avoid warning */
}

void free_pbm(struct pbm_img *img)
{
  mpz_clear(img->number);
  free(img);
}
