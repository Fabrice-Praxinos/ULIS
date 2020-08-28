// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Dispatcher.h
* @author       Clement Berthaud
* @brief        This file provides implementation for the Dispatcher utilities.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Dispatch/SpecializationCondition.h"
#include "System/Device.h"

ULIS_NAMESPACE_BEGIN
template< typename IMP >
class TDispatcher {
public:
    static ULIS_FORCEINLINE typename IMP::fpQuery Query( const FDevice& iDevice, eFormat iFormat ) {
        for( int i = 0; i < IMP::spec_size; ++i ) {
            if( IMP::spec_table[i].select_cond( iFormat ) ) {
                #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
                    if( iDevice.HasHardwareAVX2() )
                        return  IMP::spec_table[i].select_AVX;
                    else
                #endif
                #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
                    if( iDevice.HasHardwareSSE42() )
                        return  IMP::spec_table[i].select_SSE;
                    else
                #endif
                        return  IMP::spec_table[i].select_MEM;
            }
        }

        #define TMP_CALL( _TYPE_ID, _E0, _E2, _E3 ) return  QueryGeneric< _E0 >( iDevice, iFormat );
        ULIS_SWITCH_FOR_ALL_DO( iFormat.TP, ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
        #undef TMP_CALL

        ULIS_ASSERT( false, "No Dispatch found." );
        return  nullptr;
    }

private:
    template< typename T >
    static ULIS_FORCEINLINE typename IMP::fpQuery QueryGeneric( const FDevice& iDevice, eFormat iFormat ) {
        #ifdef ULIS_COMPILETIME_AVX2_SUPPORT
            if( iDevice.HasHardwareAVX2() )
                return  IMP:: template TGenericDispatchGroup< T >::select_AVX_Generic;
            else
        #endif
        #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
            if( iDevice.HasHardwareSSE42() )
                return  IMP:: template TGenericDispatchGroup< T >::select_SSE_Generic;
            else
        #endif
                return  IMP:: template TGenericDispatchGroup< T >::select_MEM_Generic;
    }
};

/////////////////////////////////////////////////////
// Macro Helper for Dispatcher definition
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                     \
    template< typename T > const typename TAG::fpQuery TAG::TGenericDispatchGroup< T >::select_AVX_Generic = AVX;
#else
    #define ULIS_DISPATCH_SELECT_GENAVX( TAG, AVX )                                                                     \
    template< typename T > const typename TAG::fpQuery TAG::TGenericDispatchGroup< T >::select_AVX_Generic = nullptr;
#endif

#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, SSE )                                                                     \
    template< typename T > const typename TAG::fpQuery TAG::TGenericDispatchGroup< T >::select_SSE_Generic = SSE;
#else
    #define ULIS_DISPATCH_SELECT_GENSSE( TAG, AVX )                                                                     \
    template< typename T > const typename TAG::fpQuery TAG::TGenericDispatchGroup< T >::select_SSE_Generic = nullptr;
#endif

#define ULIS_DISPATCH_SELECT_GENMEM( TAG, MEM )                                                                         \
    template< typename T > const typename TAG::fpQuery TAG::TGenericDispatchGroup< T >::select_MEM_Generic = MEM;

#define ULIS_BEGIN_DISPATCHER( TAG, FPT, GENAVX, GENSSE, GENMEM )   \
struct TAG {                                                        \
    typedef FPT fpQuery;                                            \
    struct FSpecDispatchGroup {                                     \
        const fpCond    select_cond;                                \
        const fpQuery  select_AVX;                                  \
        const fpQuery  select_SSE;                                  \
        const fpQuery  select_MEM;                                  \
    };                                                              \
    static const FSpecDispatchGroup spec_table[];                   \
    static const int spec_size;                                     \
    template< typename T >                                          \
    struct TGenericDispatchGroup {                                  \
        static const fpQuery select_AVX_Generic;                    \
        static const fpQuery select_SSE_Generic;                    \
        static const fpQuery select_MEM_Generic;                    \
    };                                                              \
};                                                                  \
ULIS_DISPATCH_SELECT_GENAVX( TAG, GENAVX );                         \
ULIS_DISPATCH_SELECT_GENSSE( TAG, GENSSE );                         \
ULIS_DISPATCH_SELECT_GENMEM( TAG, GENMEM );                         \
const typename TAG::FSpecDispatchGroup  TAG::spec_table[] = {

#define ULIS_BEGIN_DISPATCHER_GENERIC( TAG, FPT, GENMEM )  ULIS_BEGIN_DISPATCHER( TAG, FPT, GENMEM, GENMEM, GENMEM )

#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, _AVX, _SSE, _MEM },
    #else
        #define ULIS_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #endif
#else
    #ifdef ULIS_COMPILETIME_SSE42_SUPPORT
        #define ULIS_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, _SSE, _MEM },
    #else
        #define ULIS_DECL_DISPATCH_SPEC( _COND, _AVX, _SSE, _MEM ) { _COND, nullptr, nullptr, _MEM },
    #endif
#endif

#define ULIS_END_DISPATCHER( TAG )                                                              \
    { nullptr, nullptr, nullptr, nullptr }                                                      \
};                                                                                              \
const int TAG::spec_size = sizeof( TAG::spec_table ) / sizeof( TAG::FSpecDispatchGroup ) - 1;

ULIS_NAMESPACE_END

