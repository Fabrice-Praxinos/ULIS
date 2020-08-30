// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         SchedulePolicy.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FSchedulePolicy class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Dispatch/SchedulePolicy.h"

ULIS_NAMESPACE_BEGIN
FSchedulePolicy::~FSchedulePolicy()
{
    // Nothing ATM
}

FSchedulePolicy::FSchedulePolicy(
      eScheduleRunPolicy iRun
    , eScheduleModePolicy iMode
    , eScheduleParameterPolicy iParam
    , uint32 iValue
)
    : mRun( iRun )
    , mMode( iMode )
    , mParameter( iParam )
    , mValue( iValue )
{
}


eScheduleRunPolicy
FSchedulePolicy::RunPolicy() const
{
    return  mRun;
}

eScheduleModePolicy
FSchedulePolicy::ModePolicy() const
{
    return  mMode;
}

eScheduleParameterPolicy
FSchedulePolicy::ParameterPolicy() const
{
    return  mParameter;
}

uint32
FSchedulePolicy::Value() const
{
    return  mValue;
}

ULIS_NAMESPACE_END

