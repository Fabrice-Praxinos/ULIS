// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMono_Misc_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Maths/Geometry.h"
#include "Blend/Modes.h"
#include "Base/Perf.h"
#include "Color/ModelStructs.h"
#include "Blend/BlendHelpers.h"
#include "Conv/Conv.h"

ULIS2_NAMESPACE_BEGIN
template< typename T >
void BlendMono_Misc_MEM( const FBlock*          iSource
                       , FBlock*                iBackdrop
                       , const FRect&           iSrcRoi
                       , const FRect&           iDstRoi
                       , const eBlendingMode    iBlendingMode
                       , const eAlphaMode       iAlphaMode
                       , const float            iOpacity )
{
}

ULIS2_NAMESPACE_END

