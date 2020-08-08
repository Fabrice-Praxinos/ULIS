// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Pixel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPixel class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Pixel.h"

ULIS_NAMESPACE_BEGIN
FPixel::~FPixel()
{
}

FPixel::FPixel( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( iData, iFormat, iColorSpace )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( const_cast< uint8* >( iData ), iFormat, iColorSpace )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const FPixel& iValue )
    : ISample( const_cast< uint8* >( iValue.mSignal ), iValue.Format(), iValue.ColorSpace() )
{
}

FPixel::FPixel( FPixel&& iValue )
    : ISample( iValue.mSignal, iValue.Format(), iValue.ColorSpace() )
{
}

FPixel&
FPixel::operator=( const FPixel& iValue ) {
    ReinterpretFormat( iValue.Format() );
    AssignColorSpace( iValue.ColorSpace() );
    mSignal = iValue.mSignal;
    return  *this;
}

void
FPixel::SetPointer( uint8* iPtr )
{
    mSignal = iPtr;
}

ULIS_NAMESPACE_END

