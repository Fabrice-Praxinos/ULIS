// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Matrix.h
* @author       Clement Berthaud
* @brief        This file provides a central point to include matrix types.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Matrix2.h"
#include "Math/Geometry/Matrix3.h"
#include "Math/Geometry/Matrix4.h"

ULIS_NAMESPACE_BEGIN
template class ULIS_API TMatrix2< float >;
template class ULIS_API TMatrix3< float >;
template class ULIS_API TMatrix4< float >;
ULIS_NAMESPACE_END

