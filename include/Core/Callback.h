// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Callback.h
* @author       Clement Berthaud
* @brief        This file provides declarations for the generic callbacks mechanisms.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
typedef void (*fpInvalidateFunction)( const FBlock* /* block */, void* /* info */, const FRectI& /* rect */ );
typedef void (*fpCleanupFunction)( uint8* /* data */, void* /* info */ );

ULIS_API void OnCleanup_FreeMemory( uint8* iData, void* iInfo );

template< typename R, typename ... Ts >
class TCallback
{
public:
    /*! Typedef tFptr for the internal function pointer type. */
    typedef R (*tFptr)( Ts ..., void* );

    /*! Default null constructor. */
    TCallback()
        : mFptr( nullptr )
        , mInfo( nullptr )
    {}

    /*! Constructor from function pointer and optional extra info. */
    TCallback( tFptr iFptr, void* iInfo = nullptr )
        : mFptr( iFptr )
        , mInfo( iInfo )
    {}

    /*!
        Call the function pointer only if set.
        No return value because we can't ensure a generic default return value if not bound.
    */
    ULIS_FORCEINLINE void ExecuteIfBound( Ts ... args ) const {
        if( mFptr )
            mFptr( args ..., mInfo );
    }

    /*!
        Call the function pointer without checking if it is set.
    */
    ULIS_FORCEINLINE R Execute( Ts ... args ) const {
        ULIS_ASSERT( mFptr, "Error: Callback not set." );
        return  mFptr( args ..., mInfo );
    }

private:
    tFptr mFptr;
    void* mInfo;
};

typedef TCallback< void, uint8* > FOnCleanup;
typedef TCallback< void, const FBlock*, const FRectI& > FOnInvalid;
template class ULIS_API TCallback< void, uint8* >;
template class ULIS_API TCallback< void, const FBlock*, const FRectI& >;

ULIS_NAMESPACE_END

