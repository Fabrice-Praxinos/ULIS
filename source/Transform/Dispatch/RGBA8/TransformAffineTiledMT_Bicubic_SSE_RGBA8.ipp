// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformAffineTiledMT_Bicubic_SSE_RGBA8.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic transform entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"
#include "Transform/Dispatch/TransformInfo.h"
#include "Transform/Dispatch/Samplers.ipp"
#include <vectorclass.h>

ULIS3_NAMESPACE_BEGIN
void
InvokeTransformAffineTiledMTProcessScanline_Bicubic_SSE_RGBA8( tByte* iDst, int32 iLine, std::shared_ptr< const _FTransformInfoPrivate > iInfo, const Vec4i iIDT ) {
    const _FTransformInfoPrivate&   info    = *iInfo;
    const FFormatInfo&              fmt     = info.destination->FormatInfo();
    tByte*                          dst     = iDst;

    glm::vec3 point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine, 1.f );
    glm::vec2 point_in_src( info.inverseTransform * point_in_dst );
    glm::vec2 src_dx( info.inverseTransform * glm::vec3( 1.f, 0.f, 0.f ) );

    Vec4f p00, p10, p20, p30;
    Vec4f p01, p11, p21, p31;
    Vec4f p02, p12, p22, p32;
    Vec4f p03, p13, p23, p33;
    Vec4f hh0, hh1, hh2, hh3;
    Vec4f res, alp;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        const int   src_x   = static_cast< int >( floor( point_in_src.x ) );
        const int   src_y   = static_cast< int >( floor( point_in_src.y ) );
        const Vec4f tx      = point_in_src.x - src_x;
        const Vec4f ty      = point_in_src.y - src_y;

        const int xm1 = FMaths::PyModulo( src_x - 1, info.src_roi.w );
        const int xp0 = FMaths::PyModulo( src_x    , info.src_roi.w );
        const int xp1 = FMaths::PyModulo( src_x + 1, info.src_roi.w );
        const int xp2 = FMaths::PyModulo( src_x + 2, info.src_roi.w );
        const int ym1 = FMaths::PyModulo( src_y - 1, info.src_roi.h );
        const int yp0 = FMaths::PyModulo( src_y    , info.src_roi.h );
        const int yp1 = FMaths::PyModulo( src_y + 1, info.src_roi.h );
        const int yp2 = FMaths::PyModulo( src_y + 2, info.src_roi.h );
        #define LOAD( X )   _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( X ) ) ) )
        #define GETPIXEL( _C, _X, _Y )                                                                                                                      \
            if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) {                                                                                      \
                const tByte* pptr = info.source->PixelPtr( _X, _Y );                                                                                        \
                Vec4f _ch = LOAD( pptr );                                                                                                                   \
                Vec4f _al = _mm_set_ps1( pptr[ fmt.AID ] );                                                                                                 \
                _C = lookup8( iIDT, ( _ch * _al ) / 255.f, _al );                                                                                           \
            } else {                                                                                                                                        \
                _C = _mm_setzero_ps();                                                                                                                      \
            }
        GETPIXEL( p00, xm1,     ym1 );  GETPIXEL( p01, xm1,     yp0 );  GETPIXEL( p02, xm1,     yp1 );  GETPIXEL( p03, xm1,     yp2 );
        GETPIXEL( p10, xp0,     ym1 );  GETPIXEL( p11, xp0,     yp0 );  GETPIXEL( p12, xp0,     yp1 );  GETPIXEL( p13, xp0,     yp2 );
        GETPIXEL( p20, xp1,     ym1 );  GETPIXEL( p21, xp1,     yp0 );  GETPIXEL( p22, xp1,     yp1 );  GETPIXEL( p23, xp1,     yp2 );
        GETPIXEL( p30, xp2,     ym1 );  GETPIXEL( p31, xp2,     yp0 );  GETPIXEL( p32, xp2,     yp1 );  GETPIXEL( p33, xp2,     yp2 );
        #undef GETPIXEL
        #undef LOAD

        hh0 = InterpCubic( p00, p10, p20, p30, tx );
        hh1 = InterpCubic( p01, p11, p21, p31, tx );
        hh2 = InterpCubic( p02, p12, p22, p32, tx );
        hh3 = InterpCubic( p03, p13, p23, p33, tx );
        res = InterpCubic( hh0, hh1, hh2, hh3, ty );
        alp = lookup4( fmt.AID, res );
        alp.insert( fmt.AID, 255.f );
        res = ( res * 255.f ) / alp;

        auto _pack = _mm_cvtps_epi32( res );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )dst = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

void
TransformAffineTiledMT_Bicubic_SSE_RGBA8( std::shared_ptr< const _FTransformInfoPrivate > iInfo ) {
    const _FTransformInfoPrivate&   info        = *iInfo;
    tByte*                          dst         = info.destination->DataPtr();
    const tSize                     dst_bps     = info.destination->BytesPerScanLine();
    const tSize                     dst_decal_y = info.dst_roi.y;
    const tSize                     dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    Vec4i idt;
    BuildRGBA8IndexTable( info.source->FormatInfo().COD, &idt );
    idt.insert( info.source->FormatInfo().AID, 4 );
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformAffineTiledMTProcessScanline_Bicubic_SSE_RGBA8
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo, idt );
}


ULIS3_NAMESPACE_END
