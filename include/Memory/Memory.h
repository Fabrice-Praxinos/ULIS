// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Memory.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the memory functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
ULIS_API void* XMalloc( uint64 iSizeBytes );
ULIS_API void XFree( void* iAlloc );
ULIS_NAMESPACE_END

