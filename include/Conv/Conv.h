// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Conv.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
ULIS_API void Conv( const ISample& iSrc, ISample& iDst );
ULIS_API FColor Conv( const ISample& iSrc, eFormat iDst );

ULIS_API void Conv( FOldThreadPool*           iOldThreadPool
                   , bool                   iBlocking
                   , uint32                 iPerfIntent
                   , const FHostDeviceInfo& iHostDeviceInfo
                   , bool                   iCallCB
                   , const FBlock*          iSource
                   , FBlock*                iDestination );

ULIS_API FBlock* XConv( FOldThreadPool*           iOldThreadPool
                       , bool                   iBlocking
                       , uint32                 iPerfIntent
                       , const FHostDeviceInfo& iHostDeviceInfo
                       , bool                   iCallCB
                       , const FBlock*          iSource
                       , eFormat                iDestinationFormat );

ULIS_NAMESPACE_END

