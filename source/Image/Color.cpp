// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Pixel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FColor classes.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Color.h"
#include "Image/Pixel.h"

ULIS_NAMESPACE_BEGIN
namespace detail {

template< typename T >
struct TColorInitializer {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
    }
};

template<>
struct TColorInitializer< uint8 > {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
        for( int i = 0; i < iValues.size(); ++i )
            oColor->SetChannel8( i, ConvType< U, uint8 >( *( iValues.begin() + i ) ) );
    }
};

template<>
struct TColorInitializer< uint16 > {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
        for( int i = 0; i < iValues.size(); ++i )
            oColor->SetChannel16( i, ConvType< U, uint16 >( *( iValues.begin() + i ) ) );
    }
};

template<>
struct TColorInitializer< uint32 > {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
        for( int i = 0; i < iValues.size(); ++i )
            oColor->SetChannel32( i, ConvType< U, uint32 >( *( iValues.begin() + i ) ) );
    }
};

template<>
struct TColorInitializer< ufloat > {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
        for( int i = 0; i < iValues.size(); ++i )
            oColor->SetChannelF( i, ConvType< U, ufloat >( *( iValues.begin() + i ) ) );
    }
};

template<>
struct TColorInitializer< udouble > {
    template< typename U >
    static
    ULIS_FORCEINLINE
    void
    Run( FColor* oColor, const std::initializer_list< U > iValues ) {
        ULIS_ASSERT( iValues.size() <= oColor->SamplesPerPixel(), "Bad input values" );
        for( int i = 0; i < iValues.size(); ++i )
            oColor->SetChannelD( i, ConvType< U, udouble >( *( iValues.begin() + i ) ) );
    }
};

} // namespace detail



FColor::~FColor()
{
    if( mSignal )
        delete [] mSignal;
}

FColor::FColor()
    : ISample( nullptr, eFormat::Format_RGBA8 )
{
    mSignal = new uint8[ BytesPerPixel() ];
    memset( mSignal, 0, BytesPerPixel() );
}

FColor::FColor( eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( nullptr, iFormat, iColorSpace )
{
    mSignal = new uint8[ BytesPerPixel() ];
    memset( mSignal, 0, BytesPerPixel() );
}

FColor::FColor( const uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( nullptr, iFormat, iColorSpace )
{
    mSignal = new uint8[ BytesPerPixel() ];
    memcpy( mSignal, iData, BytesPerPixel() );
}

FColor::FColor( const FPixel& iPixel )
    : ISample( nullptr, iPixel.Format(), iPixel.ColorSpace() )
{
    mSignal = new uint8[ BytesPerPixel() ];
    memcpy( mSignal, iPixel.Bits(), BytesPerPixel() );
}

FColor::FColor( const FColor& iValue )
    : ISample( nullptr, iValue.Format(), iValue.ColorSpace() )
{
    mSignal = new uint8[ BytesPerPixel() ];
    memcpy( mSignal, iValue.Bits(), BytesPerPixel() );
}


FColor::FColor( FColor&& iValue )
    : ISample( iValue.mSignal, iValue.Format(), iValue.ColorSpace() )
{
    mSignal = new uint8[ BytesPerPixel() ];
    iValue.mSignal = nullptr;
}

FColor&
FColor::operator=( const FColor& iOther ) {
    if( mSignal )
        delete [] mSignal;

    ReinterpretFormat( iOther.Format() );
    AssignColorSpace( iOther.ColorSpace() );
    mSignal = new uint8[ BytesPerPixel() ];
    memcpy( mSignal, iOther.Bits(), BytesPerPixel() );

    return  *this;
}


template< typename T >
FColor::FColor( eFormat iFormat, std::initializer_list< T > iValues, const FColorSpace* iColorSpace )
    : ISample( nullptr, iFormat, iColorSpace )
{
    mSignal = new uint8[ BytesPerPixel() ];
    #define TMP_CALL( iTypeID, iType, iE2, iE3 ) detail::TColorInitializer< iType >::Run< T >( this, iValues ); break;
    ULIS_SWITCH_FOR_ALL_DO( Type(), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
    #undef TMP_CALL
}

//static
FColor
FColor::RGB( uint8 iR, uint8 iG, uint8 iB, uint8 iA )
{
    return  FColor( eFormat::Format_RGBA8, { iR, iG, iB, iA } );
}

//static
FColor
FColor::GreyA8( uint8 iGrey, uint8 iA )
{
    return  FColor( eFormat::Format_GA8, { iGrey, iA } );
}

//static
FColor
FColor::GreyA16( uint16 iGrey, uint16 iA )
{
    return  FColor( eFormat::Format_GA16, { iGrey, iA } );
}

//static
FColor
FColor::GreyA32( uint32 iGrey, uint32 iA )
{
    return  FColor( eFormat::Format_GA32, { iGrey, iA } );
}

//static
FColor
FColor::GreyAF( ufloat iGrey, ufloat iA )
{
    return  FColor( eFormat::Format_GAF, { iGrey, iA } );
}

//static
FColor
FColor::GreyAD( udouble iGrey, udouble iA )
{
    return  FColor( eFormat::Format_GAD, { iGrey, iA } );
}

//static
FColor
FColor::RGBA8( uint8 iR, uint8 iG, uint8 iB, uint8 iA )
{
    return  FColor( eFormat::Format_RGBA8, { iR, iG, iB, iA } );
}

//static
FColor
FColor::RGBA16( uint16 iR, uint16 iG, uint16 iB, uint16 iA )
{
    return  FColor( eFormat::Format_RGBA16, { iR, iG, iB, iA } );
}

//static
FColor
FColor::RGBA32( uint32 iR, uint32 iG, uint32 iB, uint32 iA )
{
    return  FColor( eFormat::Format_RGBA32, { iR, iG, iB, iA } );
}

//static
FColor
FColor::RGBAF( ufloat iR, ufloat iG, ufloat iB, ufloat iA )
{
    return  FColor( eFormat::Format_RGBAF, { iR, iG, iB, iA } );
}

//static
FColor
FColor::RGBAD( udouble iR, udouble iG, udouble iB, udouble iA )
{
    return  FColor( eFormat::Format_RGBAD, { iR, iG, iB, iA } );
}

//static
FColor
FColor::HSVA8( uint8 iH, uint8 iS, uint8 iV, uint8 iA )
{
    return  FColor( eFormat::Format_HSVA8, { iH, iS, iV, iA } );
}

//static
FColor
FColor::HSVA16( uint16 iH, uint16 iS, uint16 iV, uint16 iA )
{
    return  FColor( eFormat::Format_HSVA16, { iH, iS, iV, iA } );
}

//static
FColor
FColor::HSVA32( uint32 iH, uint32 iS, uint32 iV, uint32 iA )
{
    return  FColor( eFormat::Format_HSVA32, { iH, iS, iV, iA } );
}

//static
FColor
FColor::HSVAF( ufloat iH, ufloat iS, ufloat iV, ufloat iA )
{
    return  FColor( eFormat::Format_HSVAF, { iH, iS, iV, iA } );
}

//static
FColor
FColor::HSVAD( udouble iH, udouble iS, udouble iV, udouble iA )
{
    return  FColor( eFormat::Format_HSVAD, { iH, iS, iV, iA } );
}

//static
FColor
FColor::HSLA8( uint8 iH, uint8 iS, uint8 iL, uint8 iA )
{
    return  FColor( eFormat::Format_HSLA8, { iH, iS, iL, iA } );
}

//static
FColor
FColor::HSLA16( uint16 iH, uint16 iS, uint16 iL, uint16 iA )
{
    return  FColor( eFormat::Format_HSLA16, { iH, iS, iL, iA } );
}

//static
FColor
FColor::HSLA32( uint32 iH, uint32 iS, uint32 iL, uint32 iA )
{
    return  FColor( eFormat::Format_HSLA32, { iH, iS, iL, iA } );
}

//static
FColor
FColor::HSLAF( ufloat iH, ufloat iS, ufloat iL, ufloat iA )
{
    return  FColor( eFormat::Format_HSLAF, { iH, iS, iL, iA } );
}

//static
FColor
FColor::HSLAD( udouble iH, udouble iS, udouble iL, udouble iA )
{
    return  FColor( eFormat::Format_HSLAD, { iH, iS, iL, iA } );
}

//static
FColor
FColor::CMYA8( uint8 iC, uint8 iM, uint8 iY, uint8 iA )
{
    return  FColor( eFormat::Format_CMYA8, { iC, iM, iY, iA } );
}

//static
FColor
FColor::CMYA16( uint16 iC, uint16 iM, uint16 iY, uint16 iA )
{
    return  FColor( eFormat::Format_CMYA16, { iC, iM, iY, iA } );
}

//static
FColor
FColor::CMYA32( uint32 iC, uint32 iM, uint32 iY, uint32 iA )
{
    return  FColor( eFormat::Format_CMYA32, { iC, iM, iY, iA } );
}

//static
FColor
FColor::CMYAF( ufloat iC, ufloat iM, ufloat iY, ufloat iA )
{
    return  FColor( eFormat::Format_CMYAF, { iC, iM, iY, iA } );
}

//static
FColor
FColor::CMYAD( udouble iC, udouble iM, udouble iY, udouble iA )
{
    return  FColor( eFormat::Format_CMYAD, { iC, iM, iY, iA } );
}

//static
FColor
FColor::CMYKA8( uint8 iC, uint8 iM, uint8 iY, uint8 iK, uint8 iA )
{
    return  FColor( eFormat::Format_CMYKA8, { iC, iM, iY, iK, iA } );
}

//static
FColor
FColor::CMYKA16( uint16 iC, uint16 iM, uint16 iY, uint16 iK, uint16 iA )
{
    return  FColor( eFormat::Format_CMYKA16, { iC, iM, iY, iK, iA } );
}

//static
FColor
FColor::CMYKA32( uint32 iC, uint32 iM, uint32 iY, uint32 iK, uint32 iA )
{
    return  FColor( eFormat::Format_CMYKA32, { iC, iM, iY, iK, iA } );
}

//static
FColor
FColor::CMYKAF( ufloat iC, ufloat iM, ufloat iY, ufloat iK, ufloat iA )
{
    return  FColor( eFormat::Format_CMYKAF, { iC, iM, iY, iK, iA } );
}

//static
FColor
FColor::CMYKAD( udouble iC, udouble iM, udouble iY, udouble iK, udouble iA )
{
    return  FColor( eFormat::Format_CMYKAD, { iC, iM, iY, iK, iA } );
}

//static
FColor
FColor::YUVA8( uint8 iY, uint8 iU, uint8 iV, uint8 iA )
{
    return  FColor( eFormat::Format_YUVA8, { iY, iU, iV, iA } );
}

//static
FColor
FColor::YUVA16( uint16 iY, uint16 iU, uint16 iV, uint16 iA )
{
    return  FColor( eFormat::Format_YUVA16, { iY, iU, iV, iA } );
}

//static
FColor
FColor::YUVA32( uint32 iY, uint32 iU, uint32 iV, uint32 iA )
{
    return  FColor( eFormat::Format_YUVA32, { iY, iU, iV, iA } );
}

//static
FColor
FColor::YUVAF( ufloat iY, ufloat iU, ufloat iV, ufloat iA )
{
    return  FColor( eFormat::Format_YUVAF, { iY, iU, iV, iA } );
}

//static
FColor
FColor::YUVAD( udouble iY, udouble iU, udouble iV, udouble iA )
{
    return  FColor( eFormat::Format_YUVAD, { iY, iU, iV, iA } );
}

//static
FColor
FColor::LabA8( uint8 iL, uint8 ia, uint8 ib, uint8 iA )
{
    return  FColor( eFormat::Format_LabA8, { iL, ia, ib, iA } );
}

//static
FColor
FColor::LabA16( uint16 iL, uint16 ia, uint16 ib, uint16 iA )
{
    return  FColor( eFormat::Format_LabA16, { iL, ia, ib, iA } );
}

//static
FColor
FColor::LabA32( uint32 iL, uint32 ia, uint32 ib, uint32 iA )
{
    return  FColor( eFormat::Format_LabA32, { iL, ia, ib, iA } );
}

//static
FColor
FColor::LabAF( ufloat iL, ufloat ia, ufloat ib, ufloat iA )
{
    return  FColor( eFormat::Format_LabAF, { iL, ia, ib, iA } );
}

//static
FColor
FColor::LabAD( udouble iL, udouble ia, udouble ib, udouble iA )
{
    return  FColor( eFormat::Format_LabAD, { iL, ia, ib, iA } );
}

//static
FColor
FColor::XYZA8( uint8 iX, uint8 iY, uint8 iZ, uint8 iA )
{
    return  FColor( eFormat::Format_XYZA8, { iX, iY, iZ, iA } );
}

//static
FColor
FColor::XYZA16( uint16 iX, uint16 iY, uint16 iZ, uint16 iA )
{
    return  FColor( eFormat::Format_XYZA16, { iX, iY, iZ, iA } );
}

//static
FColor
FColor::XYZA32( uint32 iX, uint32 iY, uint32 iZ, uint32 iA )
{
    return  FColor( eFormat::Format_XYZA32, { iX, iY, iZ, iA } );
}

//static
FColor
FColor::XYZAF( ufloat iX, ufloat iY, ufloat iZ, ufloat iA )
{
    return  FColor( eFormat::Format_XYZAF, { iX, iY, iZ, iA } );
}

//static
FColor
FColor::XYZAD( udouble iX, udouble iY, udouble iZ, udouble iA )
{
    return  FColor( eFormat::Format_XYZAD, { iX, iY, iZ, iA } );
}

//static
FColor
FColor::YxyA8( uint8 iY, uint8 ix, uint8 iy, uint8 iA )
{
    return  FColor( eFormat::Format_YxyA8, { iY, ix, iy, iA } );
}

//static
FColor
FColor::YxyA16( uint16 iY, uint16 ix, uint16 iy, uint16 iA )
{
    return  FColor( eFormat::Format_YxyA16, { iY, ix, iy, iA } );
}

//static
FColor
FColor::YxyA32( uint32 iY, uint32 ix, uint32 iy, uint32 iA )
{
    return  FColor( eFormat::Format_YxyA32, { iY, ix, iy, iA } );
}

//static
FColor
FColor::YxyAF( ufloat iY, ufloat ix, ufloat iy, ufloat iA )
{
    return  FColor( eFormat::Format_YxyAF, { iY, ix, iy, iA } );
}

//static
FColor
FColor::YxyAD( udouble iY, udouble ix, udouble iy, udouble iA )
{
    return  FColor( eFormat::Format_YxyAD, { iY, ix, iy, iA } );
}

template ULIS_API FColor::FColor( eFormat, std::initializer_list< int >, const FColorSpace* );
template ULIS_API FColor::FColor( eFormat, std::initializer_list< uint8 >, const FColorSpace* );
template ULIS_API FColor::FColor( eFormat, std::initializer_list< uint16 >, const FColorSpace* );
template ULIS_API FColor::FColor( eFormat, std::initializer_list< uint32 >, const FColorSpace* );
template ULIS_API FColor::FColor( eFormat, std::initializer_list< ufloat >, const FColorSpace* );
template ULIS_API FColor::FColor( eFormat, std::initializer_list< udouble >, const FColorSpace* );

ULIS_NAMESPACE_END

