// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Swap.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Swap entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Swap.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
void
InvokeSwapMTProcessScanline_MEM( uint8* iDst, uint32 iCount, uint8 iC1, uint8 iC2, uint32 iBPC, uint32 iBPP ) {
    uint8* dst = iDst;
    uint8* tmp = new uint8[iBPC];
    for( uint32 i = 0; i < iCount; ++i ) {
        memcpy( tmp, dst + iC1, iBPC );
        memcpy( dst + iC1, dst + iC2, iBPC );
        memcpy( dst + iC2, tmp, iBPC );
        dst += iBPP;
    }
    delete [] tmp;
}

void
Swap( FThreadPool*              iThreadPool
    , bool                      iBlocking
    , uint32                    iPerfIntent
    , const FHostDeviceInfo&    iHostDeviceInfo
    , bool                      iCallCB
    , FBlock*                   iDestination
    , uint8                     iChannel1
    , uint8                     iChannel2 )
{
    // Assertions
    // Assertions
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iChannel1 < iDestination->SamplesPerPixel(),  "Bad channel"                                           );
    ULIS3_ASSERT( iChannel2 < iDestination->SamplesPerPixel(),  "Bad channel"                                           );
    if( iChannel1 == iChannel2 )
        return;

    FBlock* dst = iDestination;
    const uint32 bpc = dst->BytesPerSample();
    const uint32 bpp = dst->BytesPerPixel();
    const uint32 w   = dst->Width();
    const int64 bps = dst->BytesPerScanLine();
    uint8*      dsb = dst->Bits();
    #define DST dsb + ( pLINE * bps )
    const int max = dst->Height();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , InvokeSwapMTProcessScanline_MEM, DST, w, iChannel1, iChannel2, bpc, bpp )

    dst->Dirty( iCallCB );
}

ULIS3_NAMESPACE_END

