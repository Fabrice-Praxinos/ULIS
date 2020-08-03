// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Sample.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ISample class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/ColorSpace.h"
#include "Data/Format.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      ISample
/// @brief      The ISample class is an abstract interface allowing to
///             store and manipulate pixels or colors in various formats.
/// @details    ISample is a pure virtual class that provides a common
///             interface for the FPixel and FColor classes.
///
///             The ISample has a format which specifies the type, memory
///             layout, color model, and other informations about the pixel
///             data. The underlying pixel data is always a regular contiguous
///             buffer storage that lives on RAM with no alignment requirement.
///             It is usually very small, ranging from 1 byte to 40 bytes,
///             depending on the format.
///
///             The ISample interface provides a way to access individualy
///             named samples or channels components. If accessing a channel
///             from a named component that is not compatible with the initial
///             format of the ISample, the returned value could make
///             little sense to the caller, but it is properly defined and is
///             safe to call unless accessing a named component that is out of
///             range and from another color model with a greater number of
///             channels than the true color model of the data, according to
///             the format.
///
///             There are many named components that can be accessed with a
///             suffix indicating which type is required. If accessing such
///             components with a type suffix that is not the appropriate type
///             according to the format, the function will trigger an assert
///             and crash in debug build, but it will fail silently and access
///             the buffer out of bounds in release builds, leading to
///             potential memory corruption or crashes further down the line.
///
///             Here are some examples of legal and illegal uses of ISample
///             through the derived classes:
///             \snippet data/sample.h Usage
///
///             \sa FColor
///             \sa FPixel
class ULIS3_API ISample
    : public IHasFormat
    , public IHasColorSpace
{
protected:
    /*! Virtual destructor in order to allow polymorphic manipulation of subtypes. */
    ~ISample();

    /*! Construct a ISample interface in a derived class */
    ISample( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace = nullptr );

public:
    /*! Comparison strict equal operator */
    bool operator==( const ISample& iOther ) const;

    /*! Comparison strict not equal operator */
    bool operator!=( const ISample& iOther ) const;

    /*!
    Obtain a pointer to the raw data at the base element of the underlying
    buffer.

    The data is accessible through the returned pointer and can be modified
    safely, as long as you don't dereference elements outside of the buffer
    boundaries.
    */
    uint8* Bits();

    /*!
    Obtain a const pointer to the raw data at the base element of the underlying
    buffer.

    The data is accessible through the returned pointer and can be read
    safely, as long as you don't dereference elements outside of the buffer
    boundaries.
    */
    const uint8* Bits() const;

    /*!
    Obtain the value at the redirected channel index as uint8. It is undefined
    behaviour to call this if the format is not of type uint8.
    */
    ULIS3_FORCEINLINE uint8 Channel8( uint8 iIndex ) const {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT8, "Bad type" );
        return  reinterpret_cast< uint8* >( mSignal )[ RedirectedIndex( iIndex ) ];
    }

    /*!
    Obtain the value at the redirected channel index as uint16. It is undefined
    behaviour to call this if the format is not of type uint16.
    */
    ULIS3_FORCEINLINE uint16 Channel16( uint8 iIndex ) const {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT16, "Bad type" );
        return  reinterpret_cast< uint16* >( mSignal )[ RedirectedIndex( iIndex ) ];
    }

    /*!
    Obtain the value at the redirected channel index as uint32. It is undefined
    behaviour to call this if the format is not of type uint32.
    */
    ULIS3_FORCEINLINE uint32 Channel32( uint8 iIndex ) const {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT32, "Bad type" );
        return  reinterpret_cast< uint32* >( mSignal )[ RedirectedIndex( iIndex ) ];
    }

    /*!
    Obtain the value at the redirected channel index as ufloat. It is undefined
    behaviour to call this if the format is not of type ufloat.
    */
    ULIS3_FORCEINLINE ufloat ChannelF( uint8 iIndex ) const {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UFLOAT, "Bad type" );
        return  reinterpret_cast< ufloat* >( mSignal )[ RedirectedIndex( iIndex ) ];
    }

    /*!
    Obtain the value at the redirected channel index as udouble. It is undefined
    behaviour to call this if the format is not of type udouble.
    */
    ULIS3_FORCEINLINE udouble ChannelD( uint8 iIndex ) const {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UDOUBLE, "Bad type" );
        return  reinterpret_cast< udouble* >( mSignal )[ RedirectedIndex( iIndex ) ];
    }

    /*!
    Set the value at the redirected channel index as uint8. It is undefined
    behaviour to call this if the format is not of type uint8.
    */
    ULIS3_FORCEINLINE void SetChannel8( uint8 iIndex, uint8 iValue ) {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT8, "Bad type" );
        reinterpret_cast< uint8* >( mSignal )[ RedirectedIndex( iIndex ) ] = iValue;
    }

    /*!
    Set the value at the redirected channel index as uint16. It is undefined
    behaviour to call this if the format is not of type uint16.
    */
    ULIS3_FORCEINLINE void SetChannel16( uint8 iIndex, uint16 iValue ) {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT16, "Bad type" );
        reinterpret_cast< uint16* >( mSignal )[ RedirectedIndex( iIndex ) ] = iValue;
    }

    /*!
    Set the value at the redirected channel index as uint32. It is undefined
    behaviour to call this if the format is not of type uint32.
    */
    ULIS3_FORCEINLINE void SetChannel32( uint8 iIndex, uint32 iValue ) {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UINT32, "Bad type" );
        reinterpret_cast< uint16* >( mSignal )[ RedirectedIndex( iIndex ) ] = iValue;
    }

    /*!
    Set the value at the redirected channel index as ufloat. It is undefined
    behaviour to call this if the format is not of type ufloat.
    */
    ULIS3_FORCEINLINE void SetChannelF( uint8 iIndex, ufloat iValue ) {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UFLOAT, "Bad type" );
        reinterpret_cast< ufloat* >( mSignal )[ RedirectedIndex( iIndex ) ] = iValue;
    }

    /*!
    Set the value at the redirected channel index as udouble. It is undefined
    behaviour to call this if the format is not of type udouble.
    */
    ULIS3_FORCEINLINE void SetChannelD( uint8 iIndex, udouble iValue ) {
        ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
        ULIS3_ASSERT( Type() == TYPE_UDOUBLE, "Bad type" );
        reinterpret_cast< udouble* >( mSignal )[ RedirectedIndex( iIndex ) ] = iValue;
    }

    ULIS3_FORCEINLINE uint8     Alpha8()    const { return  HasAlpha() ? Channel8( AlphaIndex() )   : MaxType< uint8 >();   }
    ULIS3_FORCEINLINE uint16    Alpha16()   const { return  HasAlpha() ? Channel16( AlphaIndex() )  : MaxType< uint16 >();  }
    ULIS3_FORCEINLINE uint32    Alpha32()   const { return  HasAlpha() ? Channel32( AlphaIndex() )  : MaxType< uint32 >();  }
    ULIS3_FORCEINLINE float     AlphaF()    const { return  HasAlpha() ? ChannelF( AlphaIndex() )   : MaxType< ufloat >();  }
    ULIS3_FORCEINLINE double    AlphaD()    const { return  HasAlpha() ? ChannelD( AlphaIndex() )   : MaxType< udouble >(); }
    ULIS3_FORCEINLINE uint8     A8()        const { return  Alpha8();   }
    ULIS3_FORCEINLINE uint16    A16()       const { return  Alpha16();  }
    ULIS3_FORCEINLINE uint32    A32()       const { return  Alpha32();  }
    ULIS3_FORCEINLINE float     AF()        const { return  AlphaF();   }
    ULIS3_FORCEINLINE double    AD()        const { return  AlphaD();   }

    ULIS3_FORCEINLINE void SetAlpha8(   uint8 iValue )  { if( HasAlpha() ) SetChannel8(  AlphaIndex(), iValue ); }
    ULIS3_FORCEINLINE void SetAlpha16(  uint16 iValue ) { if( HasAlpha() ) SetChannel16( AlphaIndex(), iValue ); }
    ULIS3_FORCEINLINE void SetAlpha32(  uint32 iValue ) { if( HasAlpha() ) SetChannel32( AlphaIndex(), iValue ); }
    ULIS3_FORCEINLINE void SetAlphaF(   float iValue )  { if( HasAlpha() ) SetChannelF(  AlphaIndex(), iValue ); }
    ULIS3_FORCEINLINE void SetAlphaD(   double iValue ) { if( HasAlpha() ) SetChannelD(  AlphaIndex(), iValue ); }
    ULIS3_FORCEINLINE void SetA8(       uint8 iValue )  { SetAlpha8(  iValue ); }
    ULIS3_FORCEINLINE void SetA16(      uint16 iValue ) { SetAlpha16( iValue ); }
    ULIS3_FORCEINLINE void SetA32(      uint32 iValue ) { SetAlpha32( iValue ); }
    ULIS3_FORCEINLINE void SetAF(       float iValue )  { SetAlphaF(  iValue ); }
    ULIS3_FORCEINLINE void SetAD(       double iValue ) { SetAlphaD(  iValue ); }

    ULIS3_FORCEINLINE uint8 Grey8()         const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 Red8()          const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 Green8()        const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 Blue8()         const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 R8()            const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 G8()            const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 B8()            const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 Hue8()          const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 Saturation8()   const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 Lightness8()    const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 Value8()        const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 Cyan8()         const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 Magenta8()      const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 Yellow8()       const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 Key8()          const { return  Channel8( 3 ); }
    ULIS3_FORCEINLINE uint8 Luma8()         const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 U8()            const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 V8()            const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 K8()            const { return  Channel8( 3 ); }
    ULIS3_FORCEINLINE uint8 X8()            const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 Y8()            const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 Z8()            const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 L8()            const { return  Channel8( 0 ); }
    ULIS3_FORCEINLINE uint8 a8()            const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 b8()            const { return  Channel8( 2 ); }
    ULIS3_FORCEINLINE uint8 x8()            const { return  Channel8( 1 ); }
    ULIS3_FORCEINLINE uint8 y8()            const { return  Channel8( 2 ); }

    ULIS3_FORCEINLINE uint16 Grey16()       const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 Red16()        const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 Green16()      const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 Blue16()       const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 R16()          const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 G16()          const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 B16()          const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 Hue16()        const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 Saturation16() const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 Lightness16()  const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 Value16()      const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 Cyan16()       const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 Magenta16()    const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 Yellow16()     const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 Key16()        const { return  Channel16( 3 ); }
    ULIS3_FORCEINLINE uint16 Luma16()       const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 U16()          const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 V16()          const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 K16()          const { return  Channel16( 3 ); }
    ULIS3_FORCEINLINE uint16 X16()          const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 Y16()          const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 Z16()          const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 L16()          const { return  Channel16( 0 ); }
    ULIS3_FORCEINLINE uint16 a16()          const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 b16()          const { return  Channel16( 2 ); }
    ULIS3_FORCEINLINE uint16 x16()          const { return  Channel16( 1 ); }
    ULIS3_FORCEINLINE uint16 y16()          const { return  Channel16( 2 ); }

    ULIS3_FORCEINLINE uint32 Grey32()       const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 Red32()        const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 Green32()      const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 Blue32()       const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 R32()          const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 G32()          const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 B32()          const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 Hue32()        const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 Saturation32() const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 Lightness32()  const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 Value32()      const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 Cyan32()       const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 Magenta32()    const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 Yellow32()     const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 Key32()        const { return  Channel32( 3 ); }
    ULIS3_FORCEINLINE uint32 Luma32()       const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 U32()          const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 V32()          const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 K32()          const { return  Channel32( 3 ); }
    ULIS3_FORCEINLINE uint32 X32()          const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 Y32()          const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 Z32()          const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 L32()          const { return  Channel32( 0 ); }
    ULIS3_FORCEINLINE uint32 a32()          const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 b32()          const { return  Channel32( 2 ); }
    ULIS3_FORCEINLINE uint32 x32()          const { return  Channel32( 1 ); }
    ULIS3_FORCEINLINE uint32 y32()          const { return  Channel32( 2 ); }

    ULIS3_FORCEINLINE float GreyF()         const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float RedF()          const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float GreenF()        const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float BlueF()         const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float RF()            const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float GF()            const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float BF()            const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float HueF()          const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float SaturationF()   const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float LightnessF()    const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float ValueF()        const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float CyanF()         const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float MagentaF()      const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float YellowF()       const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float KeyF()          const { return  ChannelF( 3 ); }
    ULIS3_FORCEINLINE float LumaF()         const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float UF()            const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float VF()            const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float KF()            const { return  ChannelF( 3 ); }
    ULIS3_FORCEINLINE float XF()            const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float YF()            const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float ZF()            const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float LF()            const { return  ChannelF( 0 ); }
    ULIS3_FORCEINLINE float aF()            const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float bF()            const { return  ChannelF( 2 ); }
    ULIS3_FORCEINLINE float xF()            const { return  ChannelF( 1 ); }
    ULIS3_FORCEINLINE float yF()            const { return  ChannelF( 2 ); }

    ULIS3_FORCEINLINE double GreyD()        const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double RedD()         const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double GreenD()       const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double BlueD()        const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double RD()           const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double GD()           const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double BD()           const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double HueD()         const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double SaturationD()  const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double LightnessD()   const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double ValueD()       const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double CyanD()        const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double MagentaD()     const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double YellowD()      const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double KeyD()         const { return  ChannelD( 3 ); }
    ULIS3_FORCEINLINE double LumaD()        const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double UD()           const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double VD()           const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double KD()           const { return  ChannelD( 3 ); }
    ULIS3_FORCEINLINE double XD()           const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double YD()           const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double ZD()           const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double LD()           const { return  ChannelD( 0 ); }
    ULIS3_FORCEINLINE double aD()           const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double bD()           const { return  ChannelD( 2 ); }
    ULIS3_FORCEINLINE double xD()           const { return  ChannelD( 1 ); }
    ULIS3_FORCEINLINE double yD()           const { return  ChannelD( 2 ); }

    ULIS3_FORCEINLINE void SetGrey8(        uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetRed8(         uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGreen8(       uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBlue8(        uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetR8(           uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetG8(           uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetB8(           uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetHue8(         uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetSaturation8(  uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetLightness8(   uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetValue8(       uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetCyan8(        uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetMagenta8(     uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetYellow8(      uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKey8(         uint8 iValue )  { SetChannel8( 3, iValue ); }
    ULIS3_FORCEINLINE void SetLuma8(        uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetU8(           uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetV8(           uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetK8(           uint8 iValue )  { SetChannel8( 3, iValue ); }
    ULIS3_FORCEINLINE void SetX8(           uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void SetY8(           uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void SetZ8(           uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void SetL8(           uint8 iValue )  { SetChannel8( 0, iValue ); }
    ULIS3_FORCEINLINE void Seta8(           uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void Setb8(           uint8 iValue )  { SetChannel8( 2, iValue ); }
    ULIS3_FORCEINLINE void Setx8(           uint8 iValue )  { SetChannel8( 1, iValue ); }
    ULIS3_FORCEINLINE void Sety8(           uint8 iValue )  { SetChannel8( 2, iValue ); }

    ULIS3_FORCEINLINE void SetGrey16(       uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetRed16(        uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGreen16(      uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBlue16(       uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetR16(          uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetG16(          uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetB16(          uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetHue16(        uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetSaturation16( uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetLightness16(  uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetValue16(      uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetCyan16(       uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetMagenta16(    uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetYellow16(     uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKey16(        uint16 iValue ) { SetChannel16( 3, iValue ); }
    ULIS3_FORCEINLINE void SetLuma16(       uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetU16(          uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetV16(          uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetK16(          uint16 iValue ) { SetChannel16( 3, iValue ); }
    ULIS3_FORCEINLINE void SetX16(          uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void SetY16(          uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void SetZ16(          uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void SetL16(          uint16 iValue ) { SetChannel16( 0, iValue ); }
    ULIS3_FORCEINLINE void Seta16(          uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void Setb16(          uint16 iValue ) { SetChannel16( 2, iValue ); }
    ULIS3_FORCEINLINE void Setx16(          uint16 iValue ) { SetChannel16( 1, iValue ); }
    ULIS3_FORCEINLINE void Sety16(          uint16 iValue ) { SetChannel16( 2, iValue ); }

    ULIS3_FORCEINLINE void SetGrey32(       uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetRed32(        uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGreen32(      uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBlue32(       uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetR32(          uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetG32(          uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetB32(          uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetHue32(        uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetSaturation32( uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetLightness32(  uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetValue32(      uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetCyan32(       uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetMagenta32(    uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetYellow32(     uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKey32(        uint32 iValue ) { SetChannel32( 3, iValue ); }
    ULIS3_FORCEINLINE void SetLuma32(       uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetU32(          uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetV32(          uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetK32(          uint32 iValue ) { SetChannel32( 3, iValue ); }
    ULIS3_FORCEINLINE void SetX32(          uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void SetY32(          uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void SetZ32(          uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void SetL32(          uint32 iValue ) { SetChannel32( 0, iValue ); }
    ULIS3_FORCEINLINE void Seta32(          uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void Setb32(          uint32 iValue ) { SetChannel32( 2, iValue ); }
    ULIS3_FORCEINLINE void Setx32(          uint32 iValue ) { SetChannel32( 1, iValue ); }
    ULIS3_FORCEINLINE void Sety32(          uint32 iValue ) { SetChannel32( 2, iValue ); }

    ULIS3_FORCEINLINE void SetGreyF(        ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetRedF(         ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGreenF(       ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBlueF(        ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetRF(           ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGF(           ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBF(           ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetHueF(         ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetSaturationF(  ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetLightnessF(   ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetValueF(       ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetCyanF(        ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetMagentaF(     ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetYellowF(      ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKeyF(         ufloat iValue ) { SetChannelF( 3, iValue ); }
    ULIS3_FORCEINLINE void SetLumaF(        ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetUF(           ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetVF(           ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKF(           ufloat iValue ) { SetChannelF( 3, iValue ); }
    ULIS3_FORCEINLINE void SetXF(           ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetYF(           ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetZF(           ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetLF(           ufloat iValue ) { SetChannelF( 0, iValue ); }
    ULIS3_FORCEINLINE void SetaF(           ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetbF(           ufloat iValue ) { SetChannelF( 2, iValue ); }
    ULIS3_FORCEINLINE void SetxF(           ufloat iValue ) { SetChannelF( 1, iValue ); }
    ULIS3_FORCEINLINE void SetyF(           ufloat iValue ) { SetChannelF( 2, iValue ); }

    ULIS3_FORCEINLINE void SetGreyD(        udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetRedD(         udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGreenD(       udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBlueD(        udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetRD(           udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetGD(           udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetBD(           udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetHueD(         udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetSaturationD(  udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetLightnessD(   udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetValueD(       udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetCyanD(        udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetMagentaD(     udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetYellowD(      udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKeyD(         udouble iValue ) { SetChannelD( 3, iValue ); }
    ULIS3_FORCEINLINE void SetLumaD(        udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetUD(           udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetVD(           udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetKD(           udouble iValue ) { SetChannelD( 3, iValue ); }
    ULIS3_FORCEINLINE void SetXD(           udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetYD(           udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetZD(           udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetLD(           udouble iValue ) { SetChannelD( 0, iValue ); }
    ULIS3_FORCEINLINE void SetaD(           udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetbD(           udouble iValue ) { SetChannelD( 2, iValue ); }
    ULIS3_FORCEINLINE void SetxD(           udouble iValue ) { SetChannelD( 1, iValue ); }
    ULIS3_FORCEINLINE void SetyD(           udouble iValue ) { SetChannelD( 2, iValue ); }

protected:
    uint8* mSignal;
};

ULIS3_NAMESPACE_END

