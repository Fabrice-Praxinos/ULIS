// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         FDecl.h
* @author       Clement Berthaud
* @brief        This file provides core forward declarations for the ULIS3 library.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Platform.h"

ULIS_NAMESPACE_BEGIN
class   FRasterImage2D;
class   ISample;
class   FColor;
class   FPixel;
class   FProfileRegistry;
class   FColorSpace;
class   FThreadPool;
class   FFontRegistry;
class   FTextEngine;
class   FFont;
class   FTransformation2D;
class   ITiledBlock;
class   FKernel;
class   FStructuringElement;
struct  FPerfInfo;
struct  FHostDeviceInfo;
struct  FFormat;
struct  FTileElement;
class   ITiledBlock;
class   ITilePool;
class   FString;
class   FWString;

template< typename T > class TArray;

template< typename T > struct   TVector2;
typedef TVector2< int >         FVec2I;
typedef TVector2< float >       FVec2F;
typedef TVector2< double >      FVec2D;
typedef TVector2< uint8 >       FVec2UI8;
typedef TVector2< uint16 >      FVec2UI16;
typedef TVector2< uint32 >      FVec2UI32;
typedef TVector2< uint64 >      FVec2UI64;
typedef TVector2< int32 >       FVec2I32;
typedef TVector2< int64 >       FVec2I64;

template< typename T > struct   TVector3;
typedef TVector3< int >         FVec3I;
typedef TVector3< float >       FVec3F;

template< typename T > struct   TVector4;
typedef TVector4< float >       FVec4F;

template< typename T > struct   TRectangle;
typedef TRectangle< int >       FRectI;
typedef TRectangle< float >     FRectF;

template< typename T > class    TMatrix2;
typedef TMatrix2< float >       FMat2F;

template< typename T > class    TMatrix3;
typedef TMatrix3< float >       FMat3F;

template< typename T > class    TMatrix4;
typedef TMatrix4< float >       FMat4F;

ULIS_NAMESPACE_END

