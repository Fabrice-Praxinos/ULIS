// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformPerspectiveMT_NN_SSE_RGBA8.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformHelpers.h"
#include "Thread/ThreadPool.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformPerspectiveMTProcessScanline_NN_SSE_RGBA8( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info    = *iInfo;
    const FFormatMetrics&      fmt     = info.destination->FormatMetrics();
    uint8*                  dst     = iDst;

    FVec2F pointInDst( static_cast< float >( info.dst_roi.x ), static_cast< float >( info.dst_roi.y + iLine ) );

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        FVec2F pointInSrc = info.inverseTransform.Project( pointInDst );
        int src_x = static_cast< int >( pointInSrc.x );
        int src_y = static_cast< int >( pointInSrc.y );
        if( src_x >= minx && src_y >= miny && src_x < maxx && src_y < maxy )
            memcpy( dst, info.source->PixelBits( src_x, src_y ), fmt.BPP );

        dst += fmt.BPP;
        pointInDst.x += 1;
    }
}

void
TransformPerspectiveMT_NN_SSE_RGBA8( std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info        = *iInfo;
    uint8*                  dst         = info.destination->Bits();
    const uint32             dst_bps     = info.destination->BytesPerScanLine();
    const uint32             dst_decal_y = info.dst_roi.y;
    const uint32             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformPerspectiveMTProcessScanline_NN_SSE_RGBA8
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS_NAMESPACE_END

