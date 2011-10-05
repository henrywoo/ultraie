// UltraIE - Yet Another IE Add-on
// Copyright (C) 2006-2010
// Simon Wu Fuheng (simonwoo2000 AT gmail.com), Singapore
// Homepage: http://www.linkedin.com/in/simonwoo
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.



/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Aug 31 07:58:09 2009
 */
/* Compiler settings for .\uufilebho.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __uufilebho_h__
#define __uufilebho_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Iuufilecom_FWD_DEFINED__
#define __Iuufilecom_FWD_DEFINED__
typedef interface Iuufilecom Iuufilecom;
#endif 	/* __Iuufilecom_FWD_DEFINED__ */


#ifndef __uufilecom_FWD_DEFINED__
#define __uufilecom_FWD_DEFINED__

#ifdef __cplusplus
typedef class uufilecom uufilecom;
#else
typedef struct uufilecom uufilecom;
#endif /* __cplusplus */

#endif 	/* __uufilecom_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __Iuufilecom_INTERFACE_DEFINED__
#define __Iuufilecom_INTERFACE_DEFINED__

/* interface Iuufilecom */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_Iuufilecom;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("72F1E96F-F937-4C3B-AABC-D3CCD84EAEC2")
    Iuufilecom : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IuufilecomVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Iuufilecom * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Iuufilecom * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Iuufilecom * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Iuufilecom * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Iuufilecom * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Iuufilecom * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Iuufilecom * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IuufilecomVtbl;

    interface Iuufilecom
    {
        CONST_VTBL struct IuufilecomVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Iuufilecom_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define Iuufilecom_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define Iuufilecom_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define Iuufilecom_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define Iuufilecom_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define Iuufilecom_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define Iuufilecom_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Iuufilecom_INTERFACE_DEFINED__ */



#ifndef __uufilebhoLib_LIBRARY_DEFINED__
#define __uufilebhoLib_LIBRARY_DEFINED__

/* library uufilebhoLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_uufilebhoLib;

EXTERN_C const CLSID CLSID_uufilecom;

#ifdef __cplusplus

class DECLSPEC_UUID("393B9138-AC53-4E93-8E5A-7B45ECC147B4")
uufilecom;
#endif
#endif /* __uufilebhoLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


