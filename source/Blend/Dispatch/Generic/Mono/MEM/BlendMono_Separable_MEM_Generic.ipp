// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMono_Separable_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Blend/Blend.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/SeparableBlendFuncF.ipp"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
ULIS2_BLENDSPEC_TEMPLATE_SIG void BlendMono_Separable_MEM_Subpixel( ULIS2_BLENDSPEC_PARAMS_SIG ) {
    uint8* xidt;
    uint8 bpc, ncc, hea, spp, bpp, aid;
    tSize roi_w, roi_h, src_bps, bdp_bps, src_jmp, bdp_jmp;
    XBuildBlendParams( iBdpROI, iSource, iBackdrop, &bpc, &ncc, &hea, &spp, &bpp, &aid, &xidt, &roi_w, &roi_h, &src_bps, &bdp_bps, &src_jmp, &bdp_jmp );
    const tByte*        src = iSource->DataPtr()   + ( iSrcROI.y * src_bps ) + ( iSrcROI.x * bpp );
    tByte*              bdp = iBackdrop->DataPtr() + ( iBdpROI.y * bdp_bps ) + ( iBdpROI.x * bpp );
    const glm::vec2&    sub = iSubpixelComponent;
    glm::vec2           bus = glm::vec2( 1.f ) - iSubpixelComponent;

    //  -------------
    //  | m00 | m10 |
    //  |_____|_____|___
    //  | m01 | m11 |
    //  |_____|_____|
    //     |  |  |
    //    vv0 | vv1  -> res
    float m11, m01, m10, m00, vv0, vv1, res;
    for( tSize y = 0; y < roi_w; ++y ) {
        m11 = m10 = vv1 = 0.f;
        for( tSize x = 0; x < roi_h; ++x ) {
            m00 = m10;
            m01 = m11;
            vv0 = vv1;
            SampleSubpixelAlphaOpt< T >( src, hea, aid, bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, vv0, &m11, &m10, &vv1, &res );
            const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
            const float alpha_src       = res * iOpacity;
            const float alpha_comp      = AlphaNormalF( alpha_src, alpha_bdp );
            const float var             = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
            float alpha_result;
            ULIS2_ASSIGN_ALPHAF( iAlphaMode, alpha_result, alpha_src, alpha_bdp );

            for( uint8 j = 0; j < ncc; ++j )
            {
                uint8 r = xidt[j];
                float srcvf = SampleSubpixelChannelPremult< T >( src, r, bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, m11, m01, m10, m00, res );
                float bdpvf = TYPE2FLOAT( bdp, r );
                #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) FLOAT2TYPE( bdp, r, SeparableCompOpF< _BM >( srcvf, bdpvf, alpha_bdp, var ) );
                ULIS2_SWITCH_FOR_ALL_DO( iBlendingMode, ULIS2_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
                #undef TMP_ASSIGN
            }

            // Assign alpha
            if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );

            // Increment ptrs by one pixel
            src += bpp;
            bdp += bpp;
        }
        // Increment ptrs jump one line
        src += src_jmp;
        bdp += bdp_jmp;
    }

    delete [] xidt;
}


ULIS2_BLENDSPEC_TEMPLATE_SIG void BlendMono_Separable_MEM( ULIS2_BLENDSPEC_PARAMS_SIG ) {
    uint8* xidt;
    uint8 bpc, ncc, hea, spp, bpp, aid;
    tSize roi_w, roi_h, src_bps, bdp_bps, src_jmp, bdp_jmp;
    XBuildBlendParams( iBdpROI, iSource, iBackdrop, &bpc, &ncc, &hea, &spp, &bpp, &aid, &xidt, &roi_w, &roi_h, &src_bps, &bdp_bps, &src_jmp, &bdp_jmp );
    const tByte*        src = iSource->DataPtr()   + ( iSrcROI.y * src_bps ) + ( iSrcROI.x * bpp );
    tByte*              bdp = iBackdrop->DataPtr() + ( iBdpROI.y * bdp_bps ) + ( iBdpROI.x * bpp );

    for( tSize y = 0; y < roi_w; ++y ) {
        for( tSize x = 0; x < roi_h; ++x ) {
            const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
            const float alpha_src       = hea ? TYPE2FLOAT( src, aid ) * iOpacity : iOpacity;
            const float alpha_comp      = AlphaNormalF( alpha_src, alpha_bdp );
            const float var             = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
            float alpha_result;
            ULIS2_ASSIGN_ALPHAF( iAlphaMode, alpha_result, alpha_src, alpha_bdp );

            for( uint8 j = 0; j < ncc; ++j )
            {
                uint8 r = xidt[j];
                float srcvf = TYPE2FLOAT( src, r );
                float bdpvf = TYPE2FLOAT( bdp, r );
                #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) FLOAT2TYPE( bdp, r, SeparableCompOpF< _BM >( srcvf, bdpvf, alpha_bdp, var ) );
                ULIS2_SWITCH_FOR_ALL_DO( iBlendingMode, ULIS2_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
                #undef TMP_ASSIGN
            }

            // Assign alpha
            if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );

            // Increment ptrs by one pixel
            src += bpp;
            bdp += bpp;
        }
        // Increment ptrs jump one line
        src += src_jmp;
        bdp += bdp_jmp;
    }

    delete [] xidt;
}

ULIS2_NAMESPACE_END

