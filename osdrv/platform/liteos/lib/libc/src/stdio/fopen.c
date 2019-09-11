/************************************************************************
 * Copyright (c) 1990, 1993
 *    The Regents of the University of California.  All rights reserved.
 * Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 ************************************************************************/

/************************************************************************
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations,
 * which might include those applicable to Huawei LiteOS of U.S. and the country
 * in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in
 * compliance with such applicable export control laws and regulations.
 ************************************************************************/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)fopen.c    8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */

#include "sys/stat.h"
#include "limits.h"
#include "fcntl.h"
#include "unistd.h"
#include "stdio.h"
#include "errno.h"
#include "local.h"
#ifdef LOSCFG_LLTSER
#include "gcov_ser.h"
#endif
FILE *
fopen(const char * __restrict file, const char * __restrict mode)
{
    FILE *fp;
    int f;
    int flags, oflags;
#ifdef LOSCFG_LLTSER
    GCOV_FOPEN(file);
#endif

    if (file == NULL)
    {
        errno = EINVAL;
        return (FILE *)NULL;
    }
    if ((flags = __sflags(mode, &oflags)) == 0)
        return ((FILE *)NULL);
    if ((fp = __sfp()) == NULL)
        return ((FILE *)NULL);
    if ((f = open(file, oflags, DEFFILEMODE)) < 0) {
        fp->_flags = 0;            /* release */
        return ((FILE *)NULL);
    }
    /*
     * File descriptors are a full int, but _file is only a short.
     * If we get a valid file descriptor that is greater than
     * SHRT_MAX, then the fd will get sign-extended into an
     * invalid file descriptor.  Handle this case by failing the
     * open.
     */
    if (f > SHRT_MAX) {
        fp->_flags = 0;            /* release */
        close(f);
        errno = EMFILE;
        return ((FILE *)NULL);
    }
    fp->_file = f;
    fp->_flags = flags;
    fp->_cookie = fp;
    fp->_read = __sread;
    fp->_write = __swrite;
    fp->_seek = __sseek;
    fp->_close = __sclose;
    /*
     * When opening in append mode, even though we use O_APPEND,
     * we need to seek to the end so that ftell() gets the right
     * answer.  If the user then alters the seek pointer, or
     * the file extends, this will fail, but there is not much
     * we can do about this.  (We could set __SAPP and check in
     * fseek and ftell.)
     */
    if (oflags & O_APPEND)
        (void)__sseek(fp, (fpos_t)0, SEEK_END);
    return (fp);
}

FILE *fopen64(const char * __restrict file, const char * __restrict mode)
{
    return fopen(file, mode);
}