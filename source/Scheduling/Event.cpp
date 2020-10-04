// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FEvent
FEvent::~FEvent()
{
    delete  d;
}

FEvent::FEvent()
    : d( new FEvent_Private() )
{
}

ULIS_NAMESPACE_END

