// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMono_NonSeparable_CM_Lab_MEM_Generic.ipp
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
ULIS2_BLENDSPEC_TEMPLATE_SIG void BlendMono_NonSeparable_CM_Lab_MEM_Subpixel( ULIS2_BLENDSPEC_PARAMS_SIG ) {
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

            float subpixel_L = SampleSubpixelChannelPremult< T >( src, xidt[0], bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, m11, m01, m10, m00, res );
            float subpixel_a = SampleSubpixelChannelPremult< T >( src, xidt[1], bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, m11, m01, m10, m00, res );
            float subpixel_b = SampleSubpixelChannelPremult< T >( src, xidt[2], bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, m11, m01, m10, m00, res );
            FLabF src_lab = { subpixel_L, subpixel_a, subpixel_b };
            FLabF bdp_lab = { TYPE2FLOAT( bdp, xidt[0] ), TYPE2FLOAT( bdp, xidt[1] ), TYPE2FLOAT( bdp, xidt[2] ) };
            FLChF src_lch = LabToLCh( src_lab );
            FLChF bdp_lch = LabToLCh( bdp_lab );
            FLChF result_lch;
            #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) result_lch = NonSeparableOpF< _BM >( src_lch, bdp_lch );
            ULIS2_SWITCH_FOR_ALL_DO( iBlendingMode, ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
            #undef TMP_ASSIGN
            FLabF result_lab = LChToLab( result_lch );

            // Compose
            for( tSize j = 0; j < ncc; ++j )
                FLOAT2TYPE( bdp, xidt[j], ComposeF( *( &(src_lab.L) + j ), *( &(bdp_lab.L) + j ), alpha_bdp, var, *( &(result_lab.L) + j ) ) );

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

ULIS2_BLENDSPEC_TEMPLATE_SIG void BlendMono_NonSeparable_CM_Lab_MEM( ULIS2_BLENDSPEC_PARAMS_SIG ) {
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

            FLabF src_lab = { TYPE2FLOAT( src, xidt[0] ), TYPE2FLOAT( src, xidt[1] ), TYPE2FLOAT( src, xidt[2] ) };
            FLabF bdp_lab = { TYPE2FLOAT( bdp, xidt[0] ), TYPE2FLOAT( bdp, xidt[1] ), TYPE2FLOAT( bdp, xidt[2] ) };
            FLChF src_lch = LabToLCh( src_lab );
            FLChF bdp_lch = LabToLCh( bdp_lab );
            FLChF result_lch;
            #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) result_lch = NonSeparableOpF< _BM >( src_lch, bdp_lch );
            ULIS2_SWITCH_FOR_ALL_DO( iBlendingMode, ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
            #undef TMP_ASSIGN
            FLabF result_lab = LChToLab( result_lch );

            // Compose
            for( tSize j = 0; j < ncc; ++j )
                FLOAT2TYPE( bdp, xidt[j], ComposeF( *( &(src_lab.L) + j ), *( &(bdp_lab.L) + j ), alpha_bdp, var, *( &(result_lab.L) + j ) ) );

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

