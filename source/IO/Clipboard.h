// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Clipboard.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

ULIS_API FBlock* XLoadFromClipboard( FOldThreadPool*              iOldThreadPool
                                    , bool                      iBlocking
                                    , uint32                    iPerfIntent
                                    , const FHardwareMetrics&    iHostDeviceInfo
                                    , bool                      iCallCB
                                    , eFormat                   iDesiredFormat );

ULIS_API void SaveToClipboard( FOldThreadPool*            iOldThreadPool
                              , bool                    iBlocking
                              , uint32                  iPerfIntent
                              , const FHardwareMetrics&  iHostDeviceInfo
                              , bool                    iCallCB
                              , const FBlock*           iSource );

ULIS_API bool ClipboardHasImageData();

ULIS_NAMESPACE_END

