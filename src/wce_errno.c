/*
 * $Id: wce_errno.c,v 1.1 2006/02/19 22:04:53 mloskot Exp $
 *
 * Global errno variable.
 *
 * Created by Mateusz Loskot (mateusz@loskot.net)
 *
 * Copyright (c) 2006 Taxus SI Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation 
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom 
 * the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 * THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * MIT License:
 * http://opensource.org/licenses/mit-license.php
 *
 * Contact:
 * Taxus SI Ltd.
 * http://www.taxussi.com.pl
 *
 */

#include <winbase.h>
#include "wce_errno.h"

#define INVALID_TLS_INDEX 0xFFFFFFFF

static int s_lTlsIndexErrno = INVALID_TLS_INDEX;
static int s_lErrnoNoMem = ENOMEM;

/*******************************************************************************
* wceex_ProcessAttached
*
* Description: allocate the TLS index for errno and initialize errno for main
*              thread
*
* Return:
*
* Reference:
*
*******************************************************************************/
void wceex_ProcessAttached(void)
{
    if (s_lTlsIndexErrno == INVALID_TLS_INDEX)
    {
        s_lTlsIndexErrno = TlsAlloc();

        if (s_lTlsIndexErrno == INVALID_TLS_INDEX)
        {
            ExitProcess(3);
        }
    }

    // attach main thread
    wceex_ThreadAttached();
}

/*******************************************************************************
* wceex_ProcessDetached
*
* Description: cleanup errno for main thread and free TLS index for errno
*
* Return:
*
* Reference:
*
*******************************************************************************/
void wceex_ProcessDetached(void)
{
    // detach main thread
    wceex_ThreadDetached();

    TlsFree(s_lTlsIndexErrno);
}

/*******************************************************************************
* wceex_ThreadAttached
*
* Description: allocate and initialize errno for new thread
*
* Return:
*
* Reference:
*
*******************************************************************************/
void wceex_ThreadAttached(void)
{
    // allocate and initialize errno
    int* pErrno = (int*)malloc(sizeof(int));

    if (pErrno == 0)
    {
        ExitProcess(3);
    }

    *pErrno = 0;
    TlsSetValue(s_lTlsIndexErrno, pErrno);
}

/*******************************************************************************
* wceex_ThreadDetached
*
* Description: free errno for this thread
*
* Return:
*
* Reference:
*
*******************************************************************************/
void wceex_ThreadDetached(void)
{
    // free errno
    free (TlsGetValue(s_lTlsIndexErrno));
}

/*******************************************************************************
* wceex_errno
*
* Description: Get errno for current thread. Note: this method is thread safe.
*
* Return: pointer to errno for this thread
*
* Reference:
*
*******************************************************************************/
int* wceex_errno(void)
{
    int* pErrno = TlsGetValue(s_lTlsIndexErrno);

    if (pErrno == 0)
    {
        return &s_lErrnoNoMem;
    }
    else
    {
        return pErrno;
    }
}
