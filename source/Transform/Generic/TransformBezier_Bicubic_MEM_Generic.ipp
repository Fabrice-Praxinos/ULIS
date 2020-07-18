// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformBezier_Bicubic_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformHelpers.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformBezierMTProcessScanline_Bicubic_MEM_Generic( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask ) {
    const FTransformArgs&   info    = *iInfo;
    const FFormat&      fmt     = info.destination->FormatInfo();
    uint8*                  dst     = iDst;
    const float*            field   = reinterpret_cast< const float* >( iField->ScanlinePtr( iLine ) );
    const uint8*            mask    = reinterpret_cast< const uint8* >( iMask->ScanlinePtr( iLine ) );
    const int rangex = info.src_roi.w - 1;
    const int rangey = info.src_roi.h - 1;

    uint8* p00 = new uint8[ fmt.BPP * 4 ];      uint8* p01 = new uint8[ fmt.BPP * 4 ];
    uint8* p10 = p00 + fmt.BPP;                 uint8* p11 = p01 + fmt.BPP;
    uint8* p20 = p10 + fmt.BPP;                 uint8* p21 = p11 + fmt.BPP;
    uint8* p30 = p20 + fmt.BPP;                 uint8* p31 = p21 + fmt.BPP;
    uint8* p02 = new uint8[ fmt.BPP * 4 ];      uint8* p03 = new uint8[ fmt.BPP * 4 ];
    uint8* p12 = p02 + fmt.BPP;                 uint8* p13 = p03 + fmt.BPP;
    uint8* p22 = p12 + fmt.BPP;                 uint8* p23 = p13 + fmt.BPP;
    uint8* p32 = p22 + fmt.BPP;                 uint8* p33 = p23 + fmt.BPP;
    float* hh0 = new float[ fmt.SPP * 4 ];
    float* hh1 = new float[ fmt.SPP * 4 ];
    float* hh2 = new float[ fmt.SPP * 4 ];
    float* hh3 = new float[ fmt.SPP * 4 ];
    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        if( *mask & 0xFF ) {
            float srcxf = field[0] * rangex;
            float srcyf = field[1] * rangey;
            const int   src_x = static_cast< int >( floor( srcxf ) );
            const int   src_y = static_cast< int >( floor( srcyf ) );
            const float tx      = srcxf - src_x;
            const float ty      = srcyf - src_y;

            #define GETPIXEL( _C, _X, _Y ) if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) { memcpy( _C, info.source->PixelPtr( _X, _Y ), fmt.BPP ); } else { memset( _C, 0, fmt.BPP ); }
            GETPIXEL( p00, src_x - 1, src_y - 1 );  GETPIXEL( p01, src_x - 1, src_y + 0 );  GETPIXEL( p02, src_x - 1, src_y + 1 );  GETPIXEL( p03, src_x - 1, src_y + 2 );
            GETPIXEL( p10, src_x + 0, src_y - 1 );  GETPIXEL( p11, src_x + 0, src_y + 0 );  GETPIXEL( p12, src_x + 0, src_y + 1 );  GETPIXEL( p13, src_x + 0, src_y + 2 );
            GETPIXEL( p20, src_x + 1, src_y - 1 );  GETPIXEL( p21, src_x + 1, src_y + 0 );  GETPIXEL( p22, src_x + 1, src_y + 1 );  GETPIXEL( p23, src_x + 1, src_y + 2 );
            GETPIXEL( p30, src_x + 2, src_y - 1 );  GETPIXEL( p31, src_x + 2, src_y + 0 );  GETPIXEL( p32, src_x + 2, src_y + 1 );  GETPIXEL( p33, src_x + 2, src_y + 2 );
            #undef GETPIXEL
            SampleBicubicH< T >( hh0, p00, p10, p20, p30, fmt, tx );
            SampleBicubicH< T >( hh1, p01, p11, p21, p31, fmt, tx );
            SampleBicubicH< T >( hh2, p02, p12, p22, p32, fmt, tx );
            SampleBicubicH< T >( hh3, p03, p13, p23, p33, fmt, tx );
            SampleBicubicV< T >( dst, hh0, hh1, hh2, hh3, fmt, ty );
        }

        dst += fmt.BPP;
        field += 2;
        ++mask;
    }

    delete [] p00;
    delete [] p01;
    delete [] p02;
    delete [] p03;
    delete [] hh0;
    delete [] hh1;
    delete [] hh2;
    delete [] hh3;
}

template< typename T > void
TransformBezierMT_Bicubic_MEM_Generic( std::shared_ptr< const FTransformArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask ) {
    const FTransformArgs&   info        = *iInfo;
    uint8*                  dst         = info.destination->Bits();
    const uint32             dst_bps     = info.destination->BytesPerScanLine();
    const uint32             dst_decal_y = info.dst_roi.y;
    const uint32             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformBezierMTProcessScanline_Bicubic_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo, iField, iMask );
}

ULIS3_NAMESPACE_END

