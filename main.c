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
#include <getopt.h>
#include <string.h>
#include <err.h>
#include <ftw.h>

#include "string-utils.h"
#include "version.h"
#include "verbose.h"
#include "output.h"
#include "prime.h"
#include "main.h"
#include "help.h"
#include "load.h"

static void print_help(const char *name)
{
  struct opt_help messages[] = {
    { 'h', "help",    "Show this help message" },
    { 'V', "version", "Show version information" },
    { 'v', "verbose", "Be a bit more verbose" },
#ifdef COMMIT
    { 0,   "commit",  "Display commit information" },
#endif /* COMMIT */
    { 'f', "format",  "Select output format (?/list for list)" },
    { 0, NULL, NULL }
  };

  help(name, "[options] [pbm-file]", messages);
}

int main(int argc, char *argv[])
{
  const char *prog_name, *img_path;
  struct pbm_img *img;
  int exit_status = EXIT_FAILURE;
  /* int flags       = 0; */

  enum opt {
    OPT_COMMIT = 0x100
  };

  struct option opts[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'V' },
    { "verbose", no_argument, NULL, 'v' },
#ifdef COMMIT
    { "commit", no_argument, NULL, OPT_COMMIT },
#endif /* COMMIT */
    { NULL, 0, NULL, 0 }
  };

#ifdef __Linux__
  setproctitle_init(argc, argv, environ); /* libbsd needs that */
#endif
  prog_name = basename(argv[0]);

  while(1) {
    int c = getopt_long(argc, argv, "hVvc", opts, NULL);

    if(c == -1)
      break;

    switch(c) {
    case 'v':
      set_verbose(1);
      break;
    case 'V':
      version();
      exit_status = EXIT_SUCCESS;
      goto EXIT;
#ifdef COMMIT
    case OPT_COMMIT:
      commit();
      exit_status = EXIT_SUCCESS;
      goto EXIT;
#endif /* COMMIT */
    case 'h':
      exit_status = EXIT_SUCCESS;
    default:
      print_help(prog_name);
      goto EXIT;
    }
  }

  argc -= optind;
  argv += optind;

  img_path = NULL;
  if(argc == 1)
    img_path = argv[0];
  else if(argc != 0) {
    print_help(prog_name);
    goto EXIT;
  }

  img = load_pbm(img_path);
  primify(img);
  output(img);
  free_pbm((void *)img);

  exit_status = EXIT_SUCCESS;
EXIT:
  exit(exit_status);
}
