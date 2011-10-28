

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jun 15 11:51:11 2011
 */
/* Compiler settings for _CopyPathX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___CopyPathX_h__
#define ___CopyPathX_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICopyPathContextMenu_FWD_DEFINED__
#define __ICopyPathContextMenu_FWD_DEFINED__
typedef interface ICopyPathContextMenu ICopyPathContextMenu;
#endif 	/* __ICopyPathContextMenu_FWD_DEFINED__ */


#ifndef __CCopyPathContextMenu_FWD_DEFINED__
#define __CCopyPathContextMenu_FWD_DEFINED__

#ifdef __cplusplus
typedef class CCopyPathContextMenu CCopyPathContextMenu;
#else
typedef struct CCopyPathContextMenu CCopyPathContextMenu;
#endif /* __cplusplus */

#endif 	/* __CCopyPathContextMenu_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"
#include "shldisp.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ICopyPathContextMenu_INTERFACE_DEFINED__
#define __ICopyPathContextMenu_INTERFACE_DEFINED__

/* interface ICopyPathContextMenu */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICopyPathContextMenu;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D96F8104-CE33-4DA1-BA9E-E38A9F815CAF")
    ICopyPathContextMenu : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICopyPathContextMenuVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICopyPathContextMenu * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICopyPathContextMenu * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICopyPathContextMenu * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICopyPathContextMenu * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICopyPathContextMenu * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICopyPathContextMenu * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICopyPathContextMenu * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ICopyPathContextMenuVtbl;

    interface ICopyPathContextMenu
    {
        CONST_VTBL struct ICopyPathContextMenuVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICopyPathContextMenu_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICopyPathContextMenu_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICopyPathContextMenu_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICopyPathContextMenu_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICopyPathContextMenu_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICopyPathContextMenu_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICopyPathContextMenu_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICopyPathContextMenu_INTERFACE_DEFINED__ */



#ifndef __CopyPathX_LIBRARY_DEFINED__
#define __CopyPathX_LIBRARY_DEFINED__

/* library CopyPathX */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_CopyPathX;

EXTERN_C const CLSID CLSID_CCopyPathContextMenu;

#ifdef __cplusplus

class DECLSPEC_UUID("BE21F5FB-D683-43B1-959B-DB6A83B218CB")
CCopyPathContextMenu;
#endif
#endif /* __CopyPathX_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


