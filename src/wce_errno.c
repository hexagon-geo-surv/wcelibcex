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
#include "wce_mtdll.h"

static int s_lErrnoNoMem = ENOMEM;

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
    _ptiddata ptd = getptd();

    if (ptd == NULL)
    {
        s_lErrnoNoMem = ENOMEM; // could have been overwritten by application
        return &s_lErrnoNoMem;
    }
    else
    {
        return &(ptd->_terrno);
    }
}
