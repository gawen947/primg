/* File: safe-call.c

   Copyright (c) 2011 David Hauweele <david@hauweele.net>
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
   3. Neither the name of the University nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
   OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
   SUCH DAMAGE. */

#ifdef __linux__
# define _POSIX_C_SOURCE 200809L
#endif

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <utime.h>
#include <errno.h>
#include <err.h>

#ifdef __FreeBSD__
# include <sys/capsicum.h>
#endif

#include "safe-call.h"

/* Action to do in case of error (see safe-call.h). */
void (*safecall_err_act)(const char *msg) = safecall_default_act;

void safecall_default_act(const char *msg)
{
  /* We avoid "not a string literal" warning.
     But I still think that's dumb though. */
  err(EXIT_FAILURE, "%s", msg);
}

#define SAFE_CALL0(name, erron, msg, ret)       \
  ret x ## name () {                            \
    register ret t = name ();                   \
    if(t erron)                                 \
      safecall_err_act(msg);                             \
    return t; }

#define SAFE_CALL1(name, erron, msg, ret, type) \
  ret x ## name (type arg) {                    \
    register ret t = name (arg);                \
    if(t erron)                                 \
      safecall_err_act(msg);                             \
    return t; }

#define SAFE_CALL2(name, erron, msg, ret, type1, type2) \
  ret x ## name (type1 arg1, type2 arg2) {              \
    register ret t = name (arg1, arg2);                 \
    if(t erron)                                         \
      safecall_err_act(msg);                                     \
    return t; }

#define SAFE_CALL3(name, erron, msg, ret, type1, type2, type3) \
  ret x ## name (type1 arg1, type2 arg2, type3 arg3) {         \
    register ret t = name (arg1, arg2, arg3);                  \
    if(t erron)                                                \
      safecall_err_act(msg);                                            \
    return t; }

#define SAFE_CALL4(name, erron, msg, ret, type1, type2, type3, type4) \
  ret x ## name (type1 arg1, type2 arg2, type3 arg3, type4 arg4) {    \
    register ret t = name (arg1, arg2, arg3, arg4);                   \
    if(t erron)                                                       \
      safecall_err_act(msg);                                                   \
    return t; }

SAFE_CALL0(fork, < 0, "cannot fork", int)

SAFE_CALL1(unlink, < 0, "cannot unlink", int, const char *)
SAFE_CALL1(pipe, < 0, "cannot create pipe", int, int *)
SAFE_CALL1(malloc, == NULL, "out of memory", void *, size_t)
SAFE_CALL1(chdir, < 0, "cannot change directory", int, const char *)
SAFE_CALL1(strdup, == NULL, "out of memory", char *, const char *)

SAFE_CALL2(link, < 0, "cannot create link", int, const char *, const char *)
SAFE_CALL2(realloc, == NULL, "out of memory", void *, void *, size_t)
SAFE_CALL2(stat, < 0, "IO stat error", int, const char *, struct stat *)
SAFE_CALL2(dup2, < 0, "cannot duplicate file descriptors", int, int, int)
SAFE_CALL2(getcwd, == NULL, "cannot get current working directory", char *,
           char *, size_t)
SAFE_CALL2(utime, < 0, "IO chattr error", int, const char *,
           const struct utimbuf *)
SAFE_CALL2(listen, < 0, "listen error", int, int, int)
#ifdef __FreeBSD__
SAFE_CALL2(cap_rights_limit, < 0, "capsicum limit error", int, int, const cap_rights_t *)
#endif

SAFE_CALL3(read, < 0, "IO read error", ssize_t, int, void *, size_t)
SAFE_CALL3(write, <= 0, "IO write error", ssize_t, int, const void *, size_t)
SAFE_CALL3(open, < 0, "cannot open", int, const char *, int, mode_t)
SAFE_CALL3(chown, < 0, "IO chown error", int, const char *, uid_t, gid_t)
SAFE_CALL3(socket, < 0, "socket creation error", int, int, int, int)
SAFE_CALL3(bind, < 0, "bind error", int, int, const struct sockaddr *, socklen_t)
SAFE_CALL3(connect, < 0, "cannot connect", int, int, const struct sockaddr *, socklen_t)
#ifdef USE_THREAD
SAFE_CALL3(sem_init, < 0, "cannot initialize semaphore", int, sem_t *, int, unsigned int)
#endif /* USE_THREAD */
SAFE_CALL3(accept, < 0, "accept error", int, int, struct sockaddr *,
           socklen_t *)

SAFE_CALL4(send, < 0, "send error", ssize_t, int, const void *, size_t, int)
SAFE_CALL4(recv, < 0, "recv error", ssize_t, int, void *, size_t, int)
