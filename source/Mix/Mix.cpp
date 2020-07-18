// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Mix.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Mix entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Mix/Mix.h"
#include "Conv/Conv.h"
#include "Data/Pixel.h"
#include "Data/Color.h"
#include "Data/Sample.h"
#include "Maths/Maths.h"
#include "Conv/srgb2linear.h"

ULIS3_NAMESPACE_BEGIN
FColor
MixRGB( ufloat iT, const ISample& iA, const ISample& iB )
{
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FColor rgbA( ULIS3_FORMAT_RGBAF );
    FColor rgbB( ULIS3_FORMAT_RGBAF );
    FColor rgbResult( ULIS3_FORMAT_RGBAF );
    Conv( iA, rgbA );
    Conv( iB, rgbB );
    rgbResult.SetRF( ( 1.f - t ) * rgbA.RF() + t * rgbB.RF() );
    rgbResult.SetGF( ( 1.f - t ) * rgbA.GF() + t * rgbB.GF() );
    rgbResult.SetBF( ( 1.f - t ) * rgbA.BF() + t * rgbB.BF() );
    rgbResult.SetAF( ( 1.f - t ) * rgbA.AF() + t * rgbB.AF() );
    return  rgbResult;
}

FColor
MixLab( ufloat iT, const ISample& iA, const ISample& iB )
{
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FColor LabA( ULIS3_FORMAT_LabAF );
    FColor LabB( ULIS3_FORMAT_LabAF );
    FColor LabResult( ULIS3_FORMAT_LabAF );
    Conv( iA, LabA );
    Conv( iB, LabB );
    LabResult.SetLF( ( 1.f - t ) * LabA.LF() + t * LabB.LF() );
    LabResult.SetaF( ( 1.f - t ) * LabA.aF() + t * LabB.aF() );
    LabResult.SetbF( ( 1.f - t ) * LabA.bF() + t * LabB.bF() );
    LabResult.SetAF( ( 1.f - t ) * LabA.AF() + t * LabB.AF() );
    return  LabResult;
}

ULIS3_NAMESPACE_END

