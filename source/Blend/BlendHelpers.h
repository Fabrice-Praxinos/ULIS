// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendHelpers.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Base/Utils.h"
#include "glm/vec2.hpp"

ULIS2_NAMESPACE_BEGIN

#define ULIS2_BLENDSPEC_TEMPLATE_SIG    template< typename T>
#define ULIS2_BLENDSPEC_PARAMS_SIG      const FBlock* iSource, FBlock* iBackdrop, const FRect& iSrcROI, const FRect& iBdpROI, const glm::vec2& iSubpixelComponent, eBlendingMode iBlendingMode, eAlphaMode iAlphaMode, ufloat iOpacity, const FPerf& iPerf

ULIS2_API ULIS2_FORCEINLINE void BuildIndexTable( uint8 iCOD, uint8 iSPP, uint8* oIDT, uint8* oAID ) {
    uint8 msp = iSPP - 1;
    switch( iCOD ) {
        case 1:  for( int i = 0; i < iSPP; ++i ) oIDT[i] = ( msp - i );                                 *oAID = 0;   break;
        case 2:  for( int i = 0; i < iSPP; ++i ) oIDT[i] = ( i + 1 ) > msp ? 0 : i + 1;                 *oAID = 0;   break;
        case 3:  for( int i = 0; i < iSPP; ++i ) oIDT[i] = ( msp - i ) - 1 < 0 ? msp : ( msp - i ) - 1; *oAID = msp; break;
        default: for( int i = 0; i < iSPP; ++i ) oIDT[i] = i;                                           *oAID = msp; break;
    }
}

ULIS2_API ULIS2_FORCEINLINE void  XBuildBlendParams( const FRect&   iROI
                                                   , const FBlock*  iSRC
                                                   , FBlock*        iBDP
                                                   , uint8*         oBPC
                                                   , uint8*         oNCC
                                                   , uint8*         oHEA
                                                   , uint8*         oSPP
                                                   , uint8*         oBPP
                                                   , uint8*         oAID
                                                   , uint8**        oIDT
                                                   , tSize*         oROI_W
                                                   , tSize*         oROI_H
                                                   , tSize*         oSRC_BPS
                                                   , tSize*         oBDP_BPS
                                                   , tSize*         oSRC_JMP
                                                   , tSize*         oBDP_JMP )
{
    tFormat fmt = iSRC->Format();
    // BPC: Bytes Per Channel
    // NCC: Num Color Channel ( Without Alpha )
    // HEA: Has Extra Alpha ( 0 = false, else true )
    // SPP: Samples Per Pixel ( Number of Channels with Alpha )
    // BPP: Bytes Per Pixel ( With Alpha )
    // NUM: Number or Pixels to treat in ROI
    // AID: Alpha redirected Index
    // IDT: Redirected channels Index table, allocated here, don't forget to delete [] it outside.
    // ROI_W: Width of region of interest
    // ROI_H: Height of region of interest
    // SRC_BPS: Bytes per scanline for src
    // BDP_BPS: Bytes per scanline for bdp
    // SRC_JMP: Jump stride in bytes for src
    // BDP_JMP: Jump stride in bytes for bdp
    *oBPC = ULIS2_R_DEPTH(    fmt );
    *oNCC = ULIS2_R_CHANNELS( fmt );
    *oHEA = ULIS2_R_ALPHA(    fmt );
    *oSPP = (*oNCC) + (*oHEA);
    *oBPP = (*oBPC) * (*oSPP);
    uint8 cod = ULIS2_R_RS( fmt ); // Reverse-Swapped layout identifier
    *oIDT = new uint8[*oSPP];
    BuildIndexTable( cod, *oSPP, *oIDT, oAID );
    *oROI_W = iROI.w;
    *oROI_H = iROI.h;
    *oSRC_BPS = iSRC->Width() * ( *oBPP );
    *oBDP_BPS = iBDP->Width() * ( *oBPP );
    *oSRC_JMP = ( iSRC->Width() - ( *oROI_W ) ) * ( *oBPP );
    *oBDP_JMP = ( iBDP->Width() - ( *oROI_W ) ) * ( *oBPP );
}

template< typename T >
ULIS2_API ULIS2_FORCEINLINE
void
SampleSubpixelAlpha( const tByte* iPtr
                   , bool iHEA
                   , uint8 iChannel
                   , uint8 iBPP
                   , uint8 iBPS
                   , int64 iX
                   , int64 iY
                   , int64 iROI_W
                   , int64 iROI_H
                   , const glm::vec2& iT
                   , const glm::vec2& iU
                   , float* oM11
                   , float* oM01
                   , float* oM10
                   , float* oM00
                   , float* oVV0
                   , float* oVV1
                   , float* oRES ) {
    if( iHEA ) {
        *oM11 = ( iY >= iROI_H || iX >= iROI_W )    ? 0.f : TYPE2FLOAT( iPtr,               iChannel );
        *oM01 = ( iY >= iROI_H || iX - 1 < 0 )      ? 0.f : TYPE2FLOAT( iPtr - iBPP,        iChannel );
        *oM10 = ( iX >= iROI_W || iY - 1 < 0 )      ? 0.f : TYPE2FLOAT( iPtr - iBPS,        iChannel );
        *oM00 = ( iX - 1 < 0 || iY - 1 < 0 )        ? 0.f : TYPE2FLOAT( iPtr - iBPP + iBPP, iChannel );
    } else {
        *oM11 = ( iY >= iROI_H || iX >= iROI_W )    ? 0.f : 1.f;
        *oM01 = ( iY >= iROI_H || iX - 1 < 0 )      ? 0.f : 1.f;
        *oM10 = ( iX >= iROI_W || iY - 1 < 0 )      ? 0.f : 1.f;
        *oM00 = ( iX - 1 < 0 || iY - 1 < 0 )        ? 0.f : 1.f;
    }

    *oVV0 = *oM00 * iT.y + *oM01 * iU.y;
    *oVV1 = *oM10 * iT.y + *oM11 * iU.y;
    *oRES = *oVV0 * iT.x + *oVV1 * iU.x;
}


template< typename T >
ULIS2_API ULIS2_FORCEINLINE
void
SampleSubpixelAlphaOpt( const tByte* iPtr, bool iHEA, uint8 iChannel, uint8 iBPP, uint8 iBPS, int64 iX, int64 iY, int64 iROI_W, int64 iROI_H, const glm::vec2& iT, const glm::vec2& iU, float iVV0, float* oM11, float* oM10, float* oVV1, float* oRES ) {
    if( iHEA ) {
        *oM11 = ( iY >= iROI_H || iX >= iROI_W )    ? 0.f : TYPE2FLOAT( iPtr,               iChannel );
        *oM10 = ( iX >= iROI_W || iY - 1 < 0 )      ? 0.f : TYPE2FLOAT( iPtr - iBPS,        iChannel );
    } else {
        *oM11 = ( iY >= iROI_H || iX >= iROI_W )    ? 0.f : 1.f;
        *oM10 = ( iX >= iROI_W || iY - 1 < 0 )      ? 0.f : 1.f;
    }
    *oVV1 = (*oM10) * iT.y + (*oM11) * iU.y;
    *oRES = iVV0 * iT.x + (*oVV1) * iU.x;
}

template< typename T >
ULIS2_API ULIS2_FORCEINLINE
float
SampleSubpixelChannelPremult( const tByte* iPtr
                            , uint8 iChannel
                            , uint8 iBPP
                            , uint8 iBPS
                            , int64 iX
                            , int64 iY
                            , int64 iROI_W
                            , int64 iROI_H
                            , const glm::vec2& iT
                            , const glm::vec2& iU
                            , float iA11
                            , float iA01
                            , float iA10
                            , float iA00
                            , float iRES ) {
    float m11 = ( iY >= iROI_H || iX >= iROI_W )    ? 0.f : TYPE2FLOAT( iPtr,               iChannel );
    float m01 = ( iY >= iROI_H || iX - 1 < 0 )      ? 0.f : TYPE2FLOAT( iPtr - iBPP,        iChannel );
    float m10 = ( iX >= iROI_W || iY - 1 < 0 )      ? 0.f : TYPE2FLOAT( iPtr - iBPS,        iChannel );
    float m00 = ( iX - 1 < 0 || iY - 1 < 0 )        ? 0.f : TYPE2FLOAT( iPtr - iBPP + iBPP, iChannel );
    float v1 = ( m00 * iA00 ) * iT.y + ( m01 * iA01 ) * iU.y;
    float v2 = ( m10 * iA10 ) * iT.y + ( m11 * iA11 ) * iU.y;
    return  iRES == 0.f ? 0.f : ( ( v1 ) * iT.x + ( v2 ) * iU.x ) / iRES;
}

ULIS2_NAMESPACE_END

