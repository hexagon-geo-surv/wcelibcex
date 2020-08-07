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

#ifndef WCE_MTDLL_H
#define WCE_MTDLL_H

#ifdef __cplusplus
extern "C" {
#endif

struct _tiddata
{
    int _terrno;                /* errno value */
    char* _asctimebuf;          /* ptr to asctime() buffer */
    void* _gmtimebuf;           /* ptr to gmtime() structure */
};

typedef struct _tiddata* _ptiddata;

// --------------------------------------------------------------------------------------------------------------------
//
_ptiddata getptd(void);

// --------------------------------------------------------------------------------------------------------------------
//
// Description: functions have to be called (e.g. in DllMain) to initialize/clean up the ptd
//
void wceex_ProcessAttached(void);
void wceex_ProcessDetached(void);
void wceex_ThreadAttached(void);
void wceex_ThreadDetached(void);

#ifdef __cplusplus
}
#endif

#endif // WCE_MTDLL_H
