/* Copyright (c) 2013-2017, David Hauweele <david@hauweele.net>
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

#ifndef _VERSION_H_
#define _VERSION_H_

#include <gawen/string.h>

#define PACKAGE "primg"
#define WEBSITE "http://github.com/gawen947/primg"

#define MAJOR_VERSION    0
#define MINOR_VERSION    2

#define VERSION stringify(MAJOR_VERSION) "." stringify(MINOR_VERSION)

#if !(defined COMMIT && defined PARTIAL_COMMIT)
# define PACKAGE_VERSION PACKAGE " v" VERSION
#else
# define PACKAGE_VERSION PACKAGE " v" VERSION " (commit: " PARTIAL_COMMIT ")"
#endif /* COMMIT */

void version(void);

#ifdef COMMIT
void commit(void);
#endif /* COMMIT */

#endif /* _VERSION_H_ */
