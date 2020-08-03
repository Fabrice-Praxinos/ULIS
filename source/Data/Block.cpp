// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Block.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FBlock class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/Block.h"

ULIS3_NAMESPACE_BEGIN
FBlock::~FBlock()
{
    mOnCleanup.ExecuteIfBound( mBitmap );
}

FBlock::FBlock(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalid& iOnInvalid
    , const FOnCleanup& iOnCleanup
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mBitmap( nullptr )
    , mWidth( iWidth )
    , mHeight( iHeight )
    , mBytesPerScanline( 0 )
    , mBytesTotal( 0 )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
{
    ULIS3_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS3_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBytesPerScanline = mWidth * FormatInfo().BPP;
    mBytesTotal = mHeight * mBytesPerScanline;

    uint32 num = mWidth * mHeight * FormatInfo().SPP;
    ULIS3_ASSERT( num != 0, "Cannot allocate a buffer of size 0" );

    mBitmap = new uint8[ mBytesTotal ];
}

FBlock::FBlock(
      uint8* iData
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalid& iOnInvalid
    , const FOnCleanup& iOnCleanup
    )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mBitmap( iData )
    , mWidth( iWidth )
    , mHeight( iHeight )
    , mBytesPerScanline( 0 )
    , mBytesTotal( 0 )
    , mOnInvalid( iOnInvalid )
    , mOnCleanup( iOnCleanup )
{
    ULIS3_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS3_ASSERT( iHeight > 0, "Height must be greater than zero" );
    mBytesPerScanline = mWidth * FormatInfo().BPP;
    mBytesTotal = mHeight * mBytesPerScanline;
}

//static
FBlock*
FBlock::XMake(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , FColorSpace* iColorSpace
    , const FOnInvalid& iOnInvalid
    , const FOnCleanup& iOnCleanup
)
{
    return  new FBlock(
          iWidth
        , iHeight
        , iFormat
        , iColorSpace
        , iOnInvalid
        , iOnCleanup
    );
}

//static
FBlock* XMake(
      uint8* iData
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , FColorSpace* iColorSpace
    , const FOnInvalid& iOnInvalid
    , const FOnCleanup& iOnCleanup
)
{
    return  new FBlock(
          iData
        , iWidth
        , iHeight
        , iFormat
        , iColorSpace
        , iOnInvalid
        , iOnCleanup
    );
}

//static
void
FBlock::XDelete( FBlock* iBlock )
{
    delete  iBlock;
}

uint8*
FBlock::Bits()
{
    return  mBitmap;
}

uint8*
FBlock::ScanlineBits( uint16 iRow )
{
    ULIS3_ASSERT( iRow >= 0 && iRow < mHeight, "Index out of range" );
    return  mBitmap + ( iRow * mBytesPerScanline );
}


uint8*
FBlock::PixelBits( uint16 iX, uint16 iY )
{
    ULIS3_ASSERT( iX >= 0 && iX < mWidth, "Index out of range" );
    ULIS3_ASSERT( iY >= 0 && iY < mHeight, "Index out of range" );
    return  mBitmap + ( iX * FormatInfo().BPP + iY * mBytesPerScanline );
}

const uint8*
FBlock::Bits() const
{
    return  mBitmap;
}

const uint8*
FBlock::ScanlineBits( uint16 iRow ) const
{
    ULIS3_ASSERT( iRow >= 0 && iRow < mHeight, "Index out of range" );
    return  mBitmap + ( iRow * mBytesPerScanline );
}

const uint8*
FBlock::PixelBits( uint16 iX, uint16 iY ) const
{
    ULIS3_ASSERT( iX >= 0 && iX < mWidth, "Index out of range" );
    ULIS3_ASSERT( iY >= 0 && iY < mHeight, "Index out of range" );
    return  mBitmap + ( iX * FormatInfo().BPP + iY * mBytesPerScanline );
}

uint16
FBlock::Width() const
{
    return  mWidth;
}


uint16
FBlock::Height() const
{
    return  mHeight;
}

uint32
FBlock::Area() const
{
    return  mWidth * mHeight;
}

FRect
FBlock::Rect() const
{
    return  FRect( 0, 0, mWidth, mHeight );
}

uint32
FBlock::BytesPerScanLine() const
{
    return  mBytesPerScanline;
}


uint64
FBlock::BytesTotal() const
{
    return  mBytesTotal;
}

void
FBlock::Dirty( bool iCall ) const
{
    Dirty( Rect(), iCall );
}


void
FBlock::Dirty( const FRect& iRect, bool iCall ) const
{
    if( !iCall )
        return;

#ifdef ULIS3_ASSERT_ENABLED
    int w = static_cast< int >( mWidth );
    int h = static_cast< int >( mHeight );
    int x1 = iRect.x;
    int y1 = iRect.y;
    int x2 = iRect.x + iRect.w;
    int y2 = iRect.y + iRect.h;
    ULIS3_ASSERT( iRect.w >= 0, "Bad dirty geometry out of range" );
    ULIS3_ASSERT( iRect.h >= 0, "Bad dirty geometry out of range" );
    ULIS3_ASSERT( x1 >= 0 && x1 < w, "Bad dirty geometry out of range" );
    ULIS3_ASSERT( y1 >= 0 && y1 < h, "Bad dirty geometry out of range" );
    ULIS3_ASSERT( x2 >= 0 && x2 < w, "Bad dirty geometry out of range" );
    ULIS3_ASSERT( y2 >= 0 && y2 < h, "Bad dirty geometry out of range" );
#endif

    mOnInvalid.ExecuteIfBound( this, iRect );
}

FColor
FBlock::Color( uint16 iX, uint16 iY ) const
{
    return  FColor( PixelBits( iX, iY ), Format(), ColorSpace() );
}

FPixel
FBlock::Pixel( uint16 iX, uint16 iY )
{
    return  FPixel( PixelBits( iX, iY ), Format(), ColorSpace() );
}


const FPixel
FBlock::Pixel( uint16 iX, uint16 iY ) const
{
    return  FPixel( PixelBits( iX, iY ), Format(), ColorSpace() );
}

void
FBlock::OnInvalid( const FOnInvalid& iOnInvalid )
{
    mOnInvalid = iOnInvalid;
}

void
FBlock::OnCleanup( const FOnCleanup& iOnCleanup )
{
    mOnCleanup = iOnCleanup;
}

void
FBlock::ReloadFromData(
      uint8* iData
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnInvalid& iOnInvalid
    , const FOnCleanup& iOnCleanup
    )
{
    ULIS3_ASSERT( iWidth  > 0, "Width must be greater than zero" );
    ULIS3_ASSERT( iHeight > 0, "Height must be greater than zero" );

    mOnCleanup.ExecuteIfBound( mBitmap );

    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );

    mBitmap = iData;
    mWidth = iWidth;
    mHeight = iHeight;
    mOnInvalid = iOnInvalid;
    mOnCleanup = iOnCleanup;

    mBytesPerScanline = mWidth * FormatInfo().BPP;
    mBytesTotal = mHeight * mBytesPerScanline;
}

ULIS3_NAMESPACE_END

