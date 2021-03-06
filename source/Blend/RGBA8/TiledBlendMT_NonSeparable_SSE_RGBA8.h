// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlendMT_NonSeparable_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Blend.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTiledBlendMTProcessScanline_NonSeparable_SSE_RGBA8(
      const FBlendJobArgs* jargs
    , const FBlendCommandArgs* cargs
);

void
ScheduleTiledBlendMT_NonSeparable_SSE_RGBA8(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
);

ULIS_NAMESPACE_END

