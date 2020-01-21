// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMono_NonSeparable_CM_Grey_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Blend/Func/NonSeparableBlendFuncF.ipp"
#include "Color/ModelStructs.h"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
template< typename T >
void BlendMono_NonSeparable_CM_Grey_MEM( const FBlock*          iSource
                                       , FBlock*                iBackdrop
                                       , const glm::uvec2&      iSrcStart
                                       , const glm::uvec2&      iDstStart
                                       , const glm::uvec2&      iSrcRoiSize
                                       , const glm::uvec2&      iDstRoiSize
                                       , const glm::vec2&       iSubpixelComponent
                                       , const eBlendingMode    iBlendingMode
                                       , const eAlphaMode       iAlphaMode
                                       , const float            iOpacity )
{
    uint8 bpc, ncc, hea, spp, bpp, aid;
    tSize bps, num;
    uint8* idt;
    BuildBlendParams( &bpc, &ncc, &hea, &spp, &bpp, &bps, &num, &aid, &idt, iSource->Format(), iSource->Width(), iDstRoiSize );
    const tByte* src = iSource->DataPtr()   + ( iSrcStart.y * bps ) + ( iSrcStart.x * bpp );
    tByte*       bdp = iBackdrop->DataPtr() + ( iDstStart.y * bps ) + ( iDstStart.x * bpp );

    for( tSize i = 0; i < num; ++i ) {
        float src_greyf = TYPE2FLOAT( src, idt[0] );
        float bdp_greyf = TYPE2FLOAT( bdp, idt[0] );

        const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
        const float alpha_src       = hea ? TYPE2FLOAT( src, aid ) * iOpacity : iOpacity;
        const float alpha_comp      = AlphaNormalF( alpha_src, alpha_bdp );
        const float var             = alpha_comp == 0 ? 0 : alpha_src / alpha_comp;
        float alpha_result;
        switch( iAlphaMode ) {
            case AM_NORMAL  : alpha_result = AlphaNormalF(  alpha_src, alpha_bdp ); break;
            case AM_ERASE   : alpha_result = AlphaEraseF(   alpha_src, alpha_bdp ); break;
            case AM_TOP     : alpha_result = AlphaTopF(     alpha_src, alpha_bdp ); break;
            case AM_BACK    : alpha_result = AlphaBackF(    alpha_src, alpha_bdp ); break;
            case AM_SUB     : alpha_result = AlphaSubF(     alpha_src, alpha_bdp ); break;
            case AM_ADD     : alpha_result = AlphaAddF(     alpha_src, alpha_bdp ); break;
            case AM_MUL     : alpha_result = AlphaMulF(     alpha_src, alpha_bdp ); break;
            case AM_MIN     : alpha_result = AlphaMinF(     alpha_src, alpha_bdp ); break;
            case AM_MAX     : alpha_result = AlphaMaxF(     alpha_src, alpha_bdp ); break;
            case AM_INVMAX  : alpha_result = AlphaInvMaxF(  alpha_src, alpha_bdp ); break;
        }

        switch( iBlendingMode ) {
            #define COMPOSE( i ) FLOAT2TYPE( bdp, idt[0], ComposeF( src_greyf, bdp_greyf, alpha_bdp, var, i( src_greyf, bdp_greyf ) ) ); break;
            case BM_DARKERCOLOR     :   COMPOSE( BlendDarkerColorF  ); break;
            case BM_LIGHTERCOLOR    :   COMPOSE( BlendLighterColorF ); break;
            case BM_HUE             :   COMPOSE( BlendHueF          ); break;
            case BM_SATURATION      :   COMPOSE( BlendSaturationF   ); break;
            case BM_COLOR           :   COMPOSE( BlendColorF        ); break;
            case BM_LUMINOSITY      :   COMPOSE( BlendLuminosityF   ); break;
            default                 :   ULIS2_ASSERT( false, "Bad Blending Mode" );
            #undef COMPOSE
        }

        // Assign alpha
        if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );

        // Increment ptrs by one pixel
        src += bpp;
        bdp += bpp;
    }

    // delete temp
    delete [] idt;
}

ULIS2_NAMESPACE_END

