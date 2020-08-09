// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Pixel.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FPixel class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Sample.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FPixel
/// @brief      The FPixel class provides a mean of storing and manipulating
///             colors in various formats.
/// @details    A FPixel does not allocates its own storage space and is not
///             responsible for its memory.
///
///             \sa ISample
///             \sa FColor
class ULIS_API FPixel final
    : public ISample
{
public:
    /*! Destructor, doesn't manage lifetime of internal buffer. */
    ~FPixel();

    /*! Constructor, from external data, format and optional colorspace. */
    FPixel( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace = nullptr );

    /*! Constructor, from const external data, format and optional colorspace. */
    FPixel( const uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace = nullptr );

    /*! Copy Constructor */
    FPixel( const FPixel& iValue );

    /*! Move Constructor */
    FPixel( FPixel&& iValue );

    /*! Assignment Operator. */
    FPixel& operator=( const FPixel& iValue );

    /*!
    Move the underlying pointer to a compatible signal, usually obtained with
    PixelBits() from the FRasterImage2D this FPixel object was initially obtained from.

    \sa FRasterImage2D
    */
    void SetPointer( uint8* iPtr );
};

ULIS_NAMESPACE_END

