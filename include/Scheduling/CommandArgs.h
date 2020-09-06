// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         CommmandArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ICommmandArgs class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      ICommandArgs
/// @brief      The ICommandArgs class provides a virtual base class to implement
///             the arguments objects for each type of operations, used in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The ICommandArgs does nothing special by itself, it is meant to
///             be used in a polymorphic way.
///
///             \sa FBlendArgs
class ICommandArgs {
public:
    /*! Destructor */
    virtual ~ICommandArgs() = 0;

    /*! Constructor */
    ICommandArgs();
};

ULIS_NAMESPACE_END

