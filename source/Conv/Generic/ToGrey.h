// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ToGrey.h
* @author       Clement Berthaud
* @brief        This file provides the definition for model conversion functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Image/Color.h"
#include "Image/Pixel.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// To Grey
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From Grey
template< typename T, typename U >
void
ConvertFormatGreyToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        iDst.SetChannelT< U >( 0, ConvType< T, U >( iSrc.GetChannelT< T >( 0 ) ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From RGB
template< typename T, typename U >
void
ConvertFormatRGBToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    while( iLen-- )
    {
        ufloat r = ConvType< T, ufloat >( iSrc.GetChannelT< T >( 0 ) );
        ufloat g = ConvType< T, ufloat >( iSrc.GetChannelT< T >( 1 ) );
        ufloat b = ConvType< T, ufloat >( iSrc.GetChannelT< T >( 2 ) );
        ufloat grey = 0.3f * r + 0.59f * g + 0.11f * b;
        iDst.SetChannelT< U >( 0, ConvType< ufloat, U >( grey ) );
        iDst.SetAlphaT< U >( ConvType< T, U >( iSrc.AlphaT< T >() ) );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSV
template< typename T, typename U >
void
ConvertFormatHSVToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From HSL
template< typename T, typename U >
void
ConvertFormatHSLToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatHSLToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From CMY
template< typename T, typename U >
void
ConvertFormatCMYToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- From CMYK
template< typename T, typename U >
void
ConvertFormatCMYKToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatCMYKToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From YUV
template< typename T, typename U >
void
ConvertFormatYUVToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYUVToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Lab
template< typename T, typename U >
void
ConvertFormatLabToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatLabToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From XYZ
template< typename T, typename U >
void
ConvertFormatXYZToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatXYZToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- From Yxy
template< typename T, typename U >
void
ConvertFormatYxyToGrey( FPixel iSrc, FPixel iDst, uint32 iLen )
{
    FColor temp( eFormat::Format_RGBAF );
    while( iLen-- )
    {
        ConvertFormatYxyToRGB< T, ufloat >( iSrc, temp, 1 );
        ConvertFormatRGBToGrey< ufloat, U >( temp, iDst, 1 );
        iSrc.Next();
        iDst.Next();
    }
}

ULIS_NAMESPACE_END

