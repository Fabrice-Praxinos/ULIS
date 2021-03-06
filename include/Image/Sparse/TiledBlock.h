// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TiledBlock.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TTiledBlock class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Sparse/Tile.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
//----------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------- eScale
enum eMicro {
    MICRO_32  = 5,
    MICRO_64  = 6,
    MICRO_128 = 7,
    MICRO_256 = 8,
};

enum eMacro {
    MACRO_4   = 2,
    MACRO_8   = 3,
    MACRO_16  = 4,
    MACRO_32  = 5,
};

/////////////////////////////////////////////////////
/// ITiledBlock
class ULIS_API ITiledBlock
{
public:
    // Construction / Destruction
    virtual ~ITiledBlock() {};

public:
    // Core API
    virtual size_t      NumRootEntries() const = 0;
    virtual bool        IsValidPixelCoordRange( int64 iValue )  const = 0;
    virtual FVec2I32    ChunkCoordinatesFromPixelCoordinates( const FVec2I64& iPos ) const = 0;
    virtual FVec2I64    PixelCoordinatesFromChunkCoordinates( const FVec2I32& iPos ) const = 0;
    virtual uint64      KeyFromChunkCoordinates( const FVec2I32& iPos ) const = 0;
    virtual uint64      KeyFromPixelCoordinates( const FVec2I64& iPos ) const = 0;
    virtual FVec2I32    ChunkCoordinatesFromKey( uint64 iKey ) const = 0;
    virtual FVec2I64    PixelCoordinatesFromKey( uint64 iKey ) const = 0;

public:
    // Block API
    virtual const FRectI&    GetOperativeGeometry() const = 0;
    virtual void            ExtendOperativeGeometryAfterMutableChange( const FRectI& iRect ) = 0;
    virtual void            SubstractOperativeGeometryAfterMutableChange( const FRectI& iRect ) = 0;
    virtual void            RecomputeRoughRootGeometry() = 0;
    virtual void            RecomputeRoughLeafGeometry() = 0;

public:
    // Tile API
    virtual const FBlock* QueryConstBlockAtPixelCoordinates( FVec2I64 iPos, FVec2I64* oLocalCoords ) const = 0;
    virtual FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( FVec2I64 iPos, FVec2I64* oLocalCoords  ) = 0;
    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) = 0;
    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) = 0;
    virtual  void Clear() = 0;
    virtual  void SanitizeNow() = 0;
};

ULIS_NAMESPACE_END
