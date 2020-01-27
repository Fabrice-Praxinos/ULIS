// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Base/Perf.h"
#include "Blend/Blend.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include <glm/common.hpp>
// Distpach
#include "Blend/Dispatch/Dispatch.ipp"

ULIS2_NAMESPACE_BEGIN
void
Blend( FThreadPool*         iPool
     , bool                 iBlocking
     , bool                 iSubpixel
     , const FBlock*        iSource
     , FBlock*              iBackdrop
     , const glm::vec2&     iDstPos
     , eBlendingMode        iBlendingMode
     , eAlphaMode           iAlphaMode
     , float                iOpacity
     , const FPerf&         iPerf
     , bool                 iCallInvalidCB )
{
    BlendRect( iPool
             , iBlocking
             , iSubpixel
             , iSource
             , iBackdrop
             , iSource->Rect()
             , iDstPos
             , iBlendingMode
             , iAlphaMode
             , iOpacity
             , iPerf
             , iCallInvalidCB );
}


void
BlendRect( FThreadPool*         iPool
         , bool                 iBlocking
         , bool                 iSubpixel
         , const FBlock*        iSource
         , FBlock*              iBackdrop
         , const FRect&         iSrcRect
         , const glm::vec2&     iDstPos
         , eBlendingMode        iBlendingMode
         , eAlphaMode           iAlphaMode
         , float                iOpacity
         , const FPerf&         iPerf
         , bool                 iCallInvalidCB )
{
    ULIS2_ASSERT( iSource,                                                      "Bad source"                        );
    ULIS2_ASSERT( iBackdrop,                                                    "Bad destination"                   );
    ULIS2_ASSERT( iSource != iBackdrop,                                         "Can not blend a block on itself"   );
    ULIS2_ASSERT( iSource->Model() == iBackdrop->Model(),                       "Models do not match"               );
    ULIS2_ASSERT( iSource->Type() == iBackdrop->Type(),                         "Types do not match"                );
    ULIS2_ASSERT( iSource->SamplesPerPixel() == iBackdrop->SamplesPerPixel(),   "Samples do not match"              );
    ULIS2_ASSERT( iSource->Reversed() == iBackdrop->Reversed(),                 "Layouts do not match"              );
    ULIS2_ASSERT( iSource->Swapped() == iBackdrop->Swapped(),                   "Layouts do not match"              );

    // Ensure the selected source rect actually fits in source dimensions.
    FRect src_roi = iSrcRect & iSource->Rect();
    // Compute coordinates of target rect in destination, with source rect dimension
    int target_xmin = iSubpixel ? static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.x ) )                : static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.x ) );
    int target_ymin = iSubpixel ? static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.y ) )                : static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.y ) );
    int target_xmax = iSubpixel ? static_cast< int >( FMaths::RoundToPositiveInfinity( iDstPos.x + src_roi.w ) )    : static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.x + src_roi.w ) );
    int target_ymax = iSubpixel ? static_cast< int >( FMaths::RoundToPositiveInfinity( iDstPos.y + src_roi.h ) )    : static_cast< int >( FMaths::RoundToNegativeInfinity( iDstPos.y + src_roi.h ) );
    FRect dst_target = FRect::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );
    // Ensure the selected target actually fits in destination
    FRect dst_fit = dst_target & iBackdrop->Rect();
    if( dst_fit.Area() <= 0 ) return;
    int fx = dst_target.x - dst_fit.x;
    int fy = dst_target.y - dst_fit.y;
    src_roi.x -= dst_target.x - dst_fit.x;
    src_roi.y -= dst_target.y - dst_fit.y;
    float       opacity = FMaths::Clamp( iOpacity, 0.f, 1.f );
    glm::vec2   subpixel_component = iSubpixel ? glm::abs( FMaths::FloatingPart( iDstPos ) ) : glm::vec2( 0.f );

    fpDispatchedBlendFunc fptr = QueryDispatchedBlendFunctionForParameters( iSource->Format(), iBlendingMode, iAlphaMode, iSubpixel, iPerf );
    if( fptr ) fptr( iPool, iSource, iBackdrop, src_roi, dst_fit, subpixel_component, iBlendingMode, iAlphaMode, opacity, iPerf );

    iBackdrop->Invalidate( dst_fit, iCallInvalidCB );
}

ULIS2_NAMESPACE_END
