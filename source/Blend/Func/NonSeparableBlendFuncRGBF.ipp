// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         NonSeparableBlendFuncRGBF.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ufloat Blending functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"
#include "Color/ModelStructs.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Non Separable Blending Modes for RGB
static ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL LumF( const FRGBF& iC ) {
    return  0.3f * iC.R + 0.59f * iC.G + 0.11f * iC.B;
}

static ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL ClipColorF( FRGBF iC ) {
    ufloat l = LumF( iC );
    ufloat n = FMaths::Min3( iC.R, iC.G, iC.B );
    ufloat x = FMaths::Max3( iC.R, iC.G, iC.B );
    if( n < 0.0f )
    {
        ufloat ln = l - n;
        iC.R = l + ( ( ( iC.R - l ) * l ) / ( ln ) );
        iC.G = l + ( ( ( iC.G - l ) * l ) / ( ln ) );
        iC.B = l + ( ( ( iC.B - l ) * l ) / ( ln ) );
    }

    if( x > 1.0f )
    {
        ufloat xl = x - l;
        ufloat ml = 1.f - l;
        iC.R = l + ( ( ( iC.R - l ) * ( ml ) ) / ( xl ) );
        iC.G = l + ( ( ( iC.G - l ) * ( ml ) ) / ( xl ) );
        iC.B = l + ( ( ( iC.B - l ) * ( ml ) ) / ( xl ) );
    }

    return  iC;
}

static ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL SetLumF( const FRGBF& iC, ufloat iL ) {
    ufloat d = iL - LumF( iC );
    FRGBF C;
    C.R = iC.R + d;
    C.G = iC.G + d;
    C.B = iC.B + d;
    return  ClipColorF( C );
}

static ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL SatF( const FRGBF& iC ) {
    return  FMaths::Max3( iC.R, iC.G, iC.B ) - FMaths::Min3( iC.R, iC.G, iC.B );
}

static ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL SetSatF( const FRGBF& iC, ufloat iS ) {
    uint8 maxIndex = iC.R > iC.G ? ( iC.R > iC.B ? 0 : 2 ) : ( iC.G > iC.B ? 1 : 2 );
    uint8 minIndex = iC.R < iC.G ? ( iC.R < iC.B ? 0 : 2 ) : ( iC.G < iC.B ? 1 : 2 );
    uint8 midIndex = 3 - maxIndex - minIndex;
    ufloat Cmax = *( ( (ufloat*)( &iC ) ) + maxIndex );
    ufloat Cmin = *( ( (ufloat*)( &iC ) ) + minIndex );
    ufloat Cmid = *( ( (ufloat*)( &iC ) ) + midIndex );
    if( Cmax > Cmin )
    {
        Cmid = ( ( ( Cmid - Cmin ) * iS ) / ( Cmax - Cmin ) );
        Cmax = iS;
    }
    else
    {
        Cmid = Cmax = 0.f;
    }
    Cmin = 0.f;
    FRGBF ret;
    *( ( (ufloat*)( &ret ) ) + maxIndex ) = Cmax;
    *( ( (ufloat*)( &ret ) ) + minIndex ) = Cmin;
    *( ( (ufloat*)( &ret ) ) + midIndex ) = Cmid;
    return  ret;
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendDarkerColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) < LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendLighterColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  LumF( iCb ) > LumF( iCs ) ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendHueF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCs, SatF( iCb ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendSaturationF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( SetSatF( iCb, SatF( iCs ) ), LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendColorF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCs, LumF( iCb ) );
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE FRGBF ULIS2_VECTORCALL BlendLuminosityF( const FRGBF& iCs, const FRGBF& iCb ) {
    return  SetLumF( iCb, LumF( iCs ) );
}

/////////////////////////////////////////////////////
// Non Separable Blending Modes for Grey, these are actually separable
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendDarkerColorF( ufloat iCs, ufloat iCb ) {
    return  iCb < iCs ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendLighterColorF( ufloat iCs, ufloat iCb ) {
    return  iCb > iCs ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendHueF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendSaturationF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendColorF( ufloat iCs, ufloat iCb ) {
    return  iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE ufloat ULIS2_VECTORCALL BlendLuminosityF( ufloat iCs, ufloat iCb ) {
    return  iCs;
}
/////////////////////////////////////////////////////
// Non Separable Blending Modes for Lab LCh
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- DarkerColor
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendDarkerColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  iCb.L < iCs.L ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- LighterColor
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendLighterColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  iCb.L > iCs.L ? iCb : iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Hue
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendHueF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCb.C, iCs.h };
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Saturation
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendSaturationF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCs.C, iCb.h };
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Color
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendColorF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCb.L, iCs.C, iCs.h };
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Luminosity
ULIS2_FORCEINLINE FLChF ULIS2_VECTORCALL BlendLuminosityF( const FLChF& iCs, const FLChF& iCb ) {
    return  { iCs.L, iCb.C, iCb.h };
}

ULIS2_NAMESPACE_END

