// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         SummedAreaTable.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the SummedAreaTable entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/SummedAreaTable.h"
#include "System/HostDeviceInfo.h"
#include "Misc/Filter.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"
#include "Misc/Dispatch/Dispatch_SAT.ipp"
#include "Misc/Dispatch/Dispatch_PremultipliedSAT.ipp"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// SAT
FRasterImage2D* XGetSummedAreaTable( FThreadPool*             iThreadPool
                           , bool                     iBlocking
                           , uint32                   iPerfIntent
                           , const FHostDeviceInfo&   iHostDeviceInfo
                           , bool                     iCallCB
                           , const FRasterImage2D*            iSource )
{
    // Assertions
    ULIS_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( iSource,                  "Bad source."                                           );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    eFormat satFormat = static_cast< eFormat >( ( ( ( iSource->Format() & ULIS_E_TYPE ) & ULIS_E_DEPTH ) ) | ULIS_W_TYPE( ULIS_TYPE_UFLOAT ) | ULIS_W_FLOATING( 1 ) | ULIS_W_DEPTH( 4 ) ) );
    eFormat test = eFormat::Format_RGBAF;
    FRasterImage2D* sat = new FRasterImage2D( iSource->Width(), iSource->Height(), satFormat );

    // Query dispatched method
    fpDispatchedSATFunc fptr = QueryDispatchedSATFunctionForParameters( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo() );
    ULIS_ASSERT( fptr, "No dispatch function found." );
    fptr( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iSource, sat );

    sat->Dirty( iCallCB );

    return  sat;
}

/////////////////////////////////////////////////////
// Premultiplied SAT
FRasterImage2D* XGetPremultipliedSummedAreaTable( FThreadPool*             iThreadPool
                                        , bool                     iBlocking
                                        , uint32                   iPerfIntent
                                        , const FHostDeviceInfo&   iHostDeviceInfo
                                        , bool                     iCallCB
                                        , const FRasterImage2D*            iSource )
{
    // Assertions
    ULIS_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( iSource,                  "Bad source."                                           );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    eFormat satFormat = static_cast< eFormat >( ( ( ( iSource->Format() & ULIS_E_TYPE ) & ULIS_E_DEPTH ) ) | ULIS_W_TYPE( ULIS_TYPE_UFLOAT ) | ULIS_W_FLOATING( 1 ) | ULIS_W_DEPTH( 4 ) ) );
    eFormat test = eFormat::Format_RGBAF;
    FRasterImage2D* sat = new FRasterImage2D( iSource->Width(), iSource->Height(), satFormat );

    // Query dispatched method
    fpDispatchedSATFunc fptr = QueryDispatchedPremultipliedSATFunctionForParameters( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo() );
    ULIS_ASSERT( fptr, "No dispatch function found." );
    fptr( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iSource, sat );

    sat->Dirty( iCallCB );

    return  sat;
}

ULIS_NAMESPACE_END

