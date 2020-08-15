// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         ToHSV.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvBuffer.h"
#include "Conv/ConvHelpers.h"
#include "Image/Color.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Math/Math.h"
#include <cmath>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To HSV
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvBufferGreyToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferGreyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvBufferRGBToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = ConvType< T, ufloat >( U2_DREF_SRC( 0 ) );
        ufloat g = ConvType< T, ufloat >( U2_DREF_SRC( 1 ) );
        ufloat b = ConvType< T, ufloat >( U2_DREF_SRC( 2 ) );
        ufloat cmin = FMath::Min3( r, g, b );
        ufloat cmax = FMath::Max3( r, g, b );
        ufloat delta = cmax - cmin;

        float h = 0.0;
        float s = 0.0;
        float v = 0.0;
        v = cmax;
        if ( delta < FMath::kEpsilonf ){
            h = s = 0.f;
        } else {
            s = (delta / cmax );
            float deltaR = ( ( ( cmax - r ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaG = ( ( ( cmax - g ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            float deltaB = ( ( ( cmax - b ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
            if( fabs( r - cmax ) < FMath::kEpsilonf )        h = deltaB - deltaG;
            else if( fabs( g - cmax ) < FMath::kEpsilonf )   h = ( 1.0f / 3.0f ) + deltaR - deltaB;
            else if( fabs( b - cmax ) < FMath::kEpsilonf )   h = ( 2.0f / 3.0f ) + deltaG - deltaR;
            if( h < 0.0f ) h += 1.0f;
            if( h > 1.0f ) h -= 1.0f;
        }

        U2_DREF_DST( 0 ) = ConvType< ufloat, U >( h );
        U2_DREF_DST( 1 ) = ConvType< ufloat, U >( s );
        U2_DREF_DST( 2 ) = ConvType< ufloat, U >( v );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvBufferHSVToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    while( iLen-- )
    {
        U2_DREF_DST( 0 ) = ConvType< T, U >( U2_DREF_SRC( 0 ) );
        U2_DREF_DST( 1 ) = ConvType< T, U >( U2_DREF_SRC( 1 ) );
        U2_DREF_DST( 2 ) = ConvType< T, U >( U2_DREF_SRC( 2 ) );
        U2_FWD_ALPHA;
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvBufferHSLToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferHSLToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvBufferCMYToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvBufferCMYKToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferCMYKToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvBufferYUVToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYUVToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvBufferLabToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferLabToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvBufferXYZToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferXYZToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvBufferYxyToHSV( const FFormat& iSrcFormat, const uint8* iSrc, const FFormat& iDstFormat, uint8* iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvBufferYxyToRGB< T, ufloat >( iSrcFormat, iSrc, temp.FormatInfo(), temp.Bits(), 1 );
        ConvBufferRGBToHSV< ufloat, U >( temp.FormatInfo(), temp.Bits(), iDstFormat, iDst, 1 );
        iSrc += iSrcFormat.BPP;
        iDst += iDstFormat.BPP;
    }
}

ULIS_NAMESPACE_END

