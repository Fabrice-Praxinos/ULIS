// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Clipboard_GenericPlatform.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Clipboard.h"

ULIS_NAMESPACE_BEGIN
FBlock* XLoadFromClipboard( FOldThreadPool*            iOldThreadPool
                          , bool                    iBlocking
                          , uint32                  iPerfIntent
                          , const FHostDeviceInfo&  iHostDeviceInfo
                          , bool                    iCallCB
                          , eFormat                 iDesiredFormat )
{
    return  nullptr;
}

void SaveToClipboard( FOldThreadPool*              iOldThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHostDeviceInfo&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource )
{
    return  void();
}

bool ClipboardHasImageData_GenericPlatform() {
    return  false;
}

ULIS_NAMESPACE_END

