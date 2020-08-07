// ====================================================================================================================
//
// Project  : wcelibceex_lib
//
// Component: 
//
// Summary  : implements per thread data structure
//
// --------------------------------------------------------------------------------------------------------------------
//
// Copyright 2016 by Leica Geosystems AG, Heerbrugg
//
// ====================================================================================================================

// ====================================================================================================================
// ===================================================  Includes   ====================================================
// ====================================================================================================================

#include "wce_mtdll.h"
#include <malloc.h>
#include <winbase.h>
#include <assert.h>
#include "wce_time.h"

// ====================================================================================================================
// =================================================   Declarations   =================================================
// ====================================================================================================================

_ptiddata createTiddata(void);
void deleteTiddata(_ptiddata);

static const size_t ASCTIMEBUF_SIZE = 26;
static DWORD s_tlsIndexPtd = TLS_OUT_OF_INDEXES;

// ====================================================================================================================
// ===============================================   Static Functions   ===============================================
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
//
_ptiddata createTiddata(void)
{
    _ptiddata ptd = (_ptiddata)malloc(sizeof(struct _tiddata));
    if (ptd != NULL)
    {
        ptd->_terrno = 0;
        ptd->_asctimebuf = (char*)malloc(ASCTIMEBUF_SIZE); // TODO lazy initialization?
        ptd->_gmtimebuf = (void*)malloc(sizeof(struct tm));
    }
    return ptd;
}

// --------------------------------------------------------------------------------------------------------------------
//
void deleteTiddata(_ptiddata ptd)
{
    if (ptd != NULL)
    {
        free(ptd->_gmtimebuf);
        free(ptd->_asctimebuf);
    }
    free(ptd);
}

// --------------------------------------------------------------------------------------------------------------------
//
// Description: allocate the TLS index for _tiddata and initialize _tiddata for main thread
//
void wceex_ProcessAttached(void)
{
    if (s_tlsIndexPtd == TLS_OUT_OF_INDEXES)
    {
        s_tlsIndexPtd = TlsAlloc();

        if (s_tlsIndexPtd == TLS_OUT_OF_INDEXES)
        {
            ExitProcess(3);
        }
    }

    // attach main thread
    wceex_ThreadAttached();
}

// --------------------------------------------------------------------------------------------------------------------
//
// Description: cleanup _tiddata for main thread and free TLS index for _tiddata
//
void wceex_ProcessDetached(void)
{
    BOOL success = FALSE;

    // detach main thread
    wceex_ThreadDetached();

    success = TlsFree(s_tlsIndexPtd);
    assert(success);
}

// --------------------------------------------------------------------------------------------------------------------
//
// Description: allocate and initialize _tiddata for new thread
//
void wceex_ThreadAttached(void)
{
    BOOL success = FALSE;
    _ptiddata ptd = createTiddata();

    if (ptd == NULL)
    {
        ExitProcess(3);
    }

    success = TlsSetValue(s_tlsIndexPtd, ptd);
    assert(success);
}

// --------------------------------------------------------------------------------------------------------------------
//
// Description: free _tiddata for this thread
//
void wceex_ThreadDetached(void)
{
    deleteTiddata(TlsGetValue(s_tlsIndexPtd));
}

// --------------------------------------------------------------------------------------------------------------------
//
_ptiddata getptd(void)
{
    return TlsGetValue(s_tlsIndexPtd);
}
