// Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Callbacks.h
* @author       Clement Berthaud
* @brief        This file provides definitions for the image callbacks.
* @copyright    Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Callbacks.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Default Cleanup Behaviour
void OnCleanup_FreeMemory( tByte* iData, void* iInfo )
{
    delete [] iData;
}

void OnCleanup_DoNothing( tByte* iData, void* iInfo )
{
}

/////////////////////////////////////////////////////
/// FOnInvalid
FOnInvalid::FOnInvalid()
    : execute( nullptr )
    , info( nullptr )
{
}


FOnInvalid::FOnInvalid( fpInvalidateFunction iInvalidateFunction, void* iInvalidateInfo )
    : execute( nullptr )
    , info( nullptr )
{
}


void
FOnInvalid::ExecuteIfBound( const FBlock* iBlock, const FRect& iRect ) const
{
    if( execute )
        execute( iBlock, info, iRect );
}


/////////////////////////////////////////////////////
/// FOnCleanup
FOnCleanup::FOnCleanup()
    : execute( nullptr )
    , info( nullptr )
{
}


FOnCleanup::FOnCleanup( fpCleanupFunction iCleanupFunction, void* iCleanupInfo )
    : execute( nullptr )
    , info( nullptr )
{
}


void
FOnCleanup::ExecuteIfBound( tByte* iData ) const
{
    if( execute )
        execute( iData, info );
}


ULIS2_NAMESPACE_END