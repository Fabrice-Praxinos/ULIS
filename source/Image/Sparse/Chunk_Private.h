// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Chunk.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TTiledBlock class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Copy/Copy.h"
#include "System/Device.h"
#include "Draw/Draw.h"
#include "Draw/Sparse/DrawSparse.h"
#include "Image/Sparse/TiledBlock_Private.h"
#include "Image/Sparse/TilePool_Private.h"
#include <static_math/static_math.h>

ULIS_NAMESPACE_BEGIN
template< uint8 _MICRO, uint8 _MACRO > class TTilePool;

static const FColor default_wireframe_debug_color  = FColor::RGBA8( 40, 80, 220 );
static const FColor dirty_wireframe_debug_color    = FColor::RGBA8( 255, 0, 0 );
static const FColor correct_wireframe_debug_color  = FColor::RGBA8( 0, 255, 0 );
static const FHardwareMetrics debug_host = FHardwareMetrics::Detect();
/////////////////////////////////////////////////////
// Enums
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- eChunkType
enum class eChunkType : uint8
{
      kData
    , kRoot
    , kQuadree
};

/////////////////////////////////////////////////////
/// FDecl
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TAbstractChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TRootChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TDataChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TQuadtreeChunk;

/////////////////////////////////////////////////////
/// TAbstractChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class  TAbstractChunk
{
    typedef  TAbstractChunk< _MICRO, _MACRO, _LOCAL >   tSelf;
    typedef TTilePool< _MICRO, _MACRO >                 tTilePool;

public:
    // Construction / Destruction
    virtual ~TAbstractChunk() {}
    TAbstractChunk() {}

public:
    // Public API
    virtual  eChunkType  Type()  const = 0;
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const = 0;
    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) = 0;
    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) = 0;
    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) = 0;
    virtual  void SanitizeNow( tTilePool* iPool ) = 0;
    virtual  FRectI GetRoughLeafGeometry( const FVec2I64& iPos ) const = 0;
protected:
    // Protected Data Members
    static constexpr uint8  micro_threshold                     = _MICRO;
    static constexpr uint8  macro_threshold                     = _MACRO;
    static constexpr uint8  local_threshold                     = _LOCAL;
    static constexpr uint8  sub_threshold                       = _LOCAL - 1;
    static constexpr uint16 micro_chunk_size_as_pixels          = ::smath::pow( 2, micro_threshold );
    static constexpr uint16 local_chunk_size_as_micro_chunks    = ::smath::pow( 2, local_threshold );
    static constexpr uint32 local_chunk_size_as_pixels          = micro_chunk_size_as_pixels * local_chunk_size_as_micro_chunks;
    static constexpr uint32 local_chunk_halfsize_as_pixels      = local_chunk_size_as_pixels / 2;
    static constexpr bool   bed                                 = _LOCAL == 1;
};

/////////////////////////////////////////////////////
/// TAbstractChunk
template< uint8 _MICRO, uint8 _MACRO >
class  TAbstractChunk< _MICRO, _MACRO, 0 >
{
    typedef  TAbstractChunk< _MICRO, _MACRO, 0 >    tSelf;
    typedef TTilePool< _MICRO, _MACRO >             tTilePool;

public:
    // Construction / Destruction
    virtual ~TAbstractChunk() {}
    TAbstractChunk() {}

public:
    // Public API
    virtual  eChunkType  Type()  const = 0;
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const = 0;
    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) = 0;
    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) = 0;
    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) = 0;
    virtual  void SanitizeNow( tTilePool* iPool ) = 0;
    virtual  FRectI GetRoughLeafGeometry( const FVec2I64& iPos ) const = 0;

protected:
    // Protected Data Members
    static constexpr uint8  micro_threshold                     = _MICRO;
    static constexpr uint8  macro_threshold                     = _MACRO;
    static constexpr uint8  local_threshold                     = 0;
    static constexpr uint8  sub_threshold                       = 0;
    static constexpr uint16 micro_chunk_size_as_pixels          = ::smath::pow( 2, micro_threshold );
    static constexpr uint16 local_chunk_size_as_micro_chunks    = ::smath::pow( 2, local_threshold );
    static constexpr uint32 local_chunk_size_as_pixels          = micro_chunk_size_as_pixels * local_chunk_size_as_micro_chunks;
    static constexpr uint32 local_chunk_halfsize_as_pixels      = local_chunk_size_as_pixels / 2;
    static constexpr bool   bed                                 = false;
};

/////////////////////////////////////////////////////
/// TRootChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class  TRootChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tSuperClass;
    typedef  TRootChunk<        _MICRO, _MACRO, _LOCAL >    tSelf;
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tChild;
    typedef  TDataChunk<        _MICRO, _MACRO, _LOCAL >    tDataChild;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, _LOCAL >    tQuadtreeChild;
    typedef TTilePool< _MICRO, _MACRO >                     tTilePool;

public:
    // Construction / Destruction
    virtual ~TRootChunk() {
        if( mChild )
            delete  mChild;
    }
    TRootChunk()
        : mChild( nullptr )
        , mDirty( true )
    {}

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kRoot; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        return  mChild ? iPool->EmptyTile() : mChild->QueryConstBlockAtPixelCoordinates( iPool, iPos );
    }


    void PerformRootSubdivisionForImminentMutableChangeIfNeeded( tTilePool* iPool ) {
        if( mChild == nullptr )
            if( tSuperClass::bed ) {
                // X Query Tile, X stands for Refcount decrease needed by caller.
                FTileElement* tile = iPool->XQueryFreshTile();
                mChild = dynamic_cast< tChild* >( new tDataChild(  ) );
                tile->DecreaseRefCount();
            } else {
                mChild = dynamic_cast< tChild* >( new tQuadtreeChild() );
            }
        else
            if( mChild->Type() == eChunkType::kData && !tSuperClass::bed ) {
                FTileElement* tile = dynamic_cast< tDataChild* >( mChild )->PointedData();
                tile->IncreaseRefCount();
                if( mChild )
                    delete  mChild;
                mChild = new tQuadtreeChild( tile );
                tile->DecreaseRefCount();
            }
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        mDirty.store( true );
        PerformRootSubdivisionForImminentMutableChangeIfNeeded( iPool );
        return  mChild->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( iPool, iPos );
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size = round( tSuperClass::local_chunk_size_as_pixels * iScale );
        DrawRectOutlineNoAA( iDst, default_wireframe_debug_color, FRectI( static_cast< int >( iPos.x )
                                                                       , static_cast< int >( iPos.y )
                                                                       , static_cast< int >( size )
                                                                       , static_cast< int >( size ) ) );
        if( mChild )
            mChild->DrawDebugWireframe( iDst, iPos, iScale );
    }

    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) override {
        if( mChild )
            mChild->DrawDebugTileContent( iDst, iPos );
    }

    virtual  void SanitizeNow( tTilePool* iPool ) override {
        if( mDirty.load() == false )
            return;

        if( mChild ) {
            mChild->SanitizeNow( iPool );
            if( mChild->Type() == eChunkType::kData ) {
                tDataChild* data = dynamic_cast< tDataChild* >( mChild );
                if( data->PointedData() == nullptr ) {
                    delete  mChild;
                    mChild = nullptr;
                }
            }
            else if( mChild->Type() == eChunkType::kQuadree ) {
                tQuadtreeChild* quad = dynamic_cast< tQuadtreeChild* >( mChild );
                FTileElement* el = nullptr;
                bool uniform = quad->CheckUniformDistributedValue( &el );
                if( uniform ) {
                    if( el == nullptr ) {
                        delete  mChild;
                        mChild = nullptr;
                    } else {
                        delete  mChild;
                        mChild = new tDataChild( el );
                    }
                }
            }
        }
    }

    virtual  FRectI GetRoughLeafGeometry( const FVec2I64& iPos ) const override {
        if( mChild )
            return  mChild->GetRoughLeafGeometry( iPos );
        else
            return  FRectI();
    }

    const tChild* Child() const {
        return  mChild;
    }

private:
    // Private Data Members
    tChild* mChild;
    std::atomic< bool > mDirty;
};


/////////////////////////////////////////////////////
/// TDataChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class  TDataChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tSuperClass;
    typedef  TDataChunk<        _MICRO, _MACRO, _LOCAL >    tSelf;
    typedef TTilePool< _MICRO, _MACRO >                 tTilePool;

public:
    // Construction / Destruction
    virtual ~TDataChunk() {
        if( mPtr )
            mPtr->DecreaseRefCount();
    }

    TDataChunk()
        : mPtr( nullptr )
    {}

    TDataChunk( FTileElement* iDistributedValue )
        : mPtr( iDistributedValue )
    {
        mPtr->IncreaseRefCount();
    }

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kData; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        return  mPtr->mBlock;
    }

    void PerformDataCopyForImminentMutableChangeIfNeeded( tTilePool* iPool ) {
        mPtr = iPool->XPerformDataCopyForImminentMutableChangeIfNeeded( mPtr );
        mPtr->mDirty = true;
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        PerformDataCopyForImminentMutableChangeIfNeeded( iPool );
        mPtr->mDirty = true;
        return  &mPtr;
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size = static_cast< int >( round( tSuperClass::local_chunk_size_as_pixels * iScale ) );
        if( mPtr->mDirty )
            DrawRectOutlineNoAA( iDst, dirty_wireframe_debug_color, FRectI( static_cast< int >( iPos.x ), static_cast< int >( iPos.y ), size, size ) );
        else
            DrawRectOutlineNoAA( iDst, correct_wireframe_debug_color, FRectI( static_cast< int >( iPos.x ), static_cast< int >( iPos.y ), size, size ) );
    }

    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) override {
        for( int i = 0; i < tSuperClass::local_chunk_size_as_pixels; i+= tSuperClass::micro_chunk_size_as_pixels ) {
            for( int j = 0; j < tSuperClass::local_chunk_size_as_pixels; j+= tSuperClass::micro_chunk_size_as_pixels ) {
                Copy( nullptr, ULIS_NONBLOCKING, 0, debug_host, ULIS_NOCB, mPtr->mBlock, iDst, mPtr->mBlock->Rect(), iPos + FVec2I64( i, j ) );
            }
        }
    }

    virtual  void SanitizeNow( tTilePool* iPool ) override {
        if( mPtr->mDirty == false )
            mPtr = iPool->PerformRedundantHashMergeReturnCorrect( mPtr );
    }

    virtual  FRectI GetRoughLeafGeometry( const FVec2I64& iPos ) const override {
        if( mPtr )
            return  FRectI( static_cast< int >( iPos.x )
                         , static_cast< int >( iPos.y )
                         , static_cast< int >( tSuperClass::local_chunk_size_as_pixels )
                         , static_cast< int >( tSuperClass::local_chunk_size_as_pixels ) );
        else
            return  FRectI();
    }

    FTileElement* PointedData() {
        return  mPtr;
    }

private:
    // Private Data Members
    FTileElement* mPtr;
};

/////////////////////////////////////////////////////
/// TQuadtreeChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class  TQuadtreeChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >                        tSuperClass;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, _LOCAL >                        tSelf;
    typedef  TAbstractChunk<    _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubAbstractChunk;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubQuadtreeChunk;
    typedef  TDataChunk<        _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubDataChunk;
    typedef TTilePool< _MICRO, _MACRO >                                         tTilePool;

public:
    // Construction / Destruction
    virtual ~TQuadtreeChunk() {
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                delete  mQuad[i];
    }

    TQuadtreeChunk()
        : mQuad{ nullptr, nullptr, nullptr, nullptr }
    {
    }

    TQuadtreeChunk( FTileElement* iDistributedValue )
        : mQuad{ nullptr, nullptr, nullptr, nullptr }
    {
        for( int i = 0; i < 4; ++i )
            mQuad[i] = new  tSubDataChunk( iDistributedValue );
    }

private:
    // Coordinates API
    FVec2I64  SubChunkCoordinatesFromLocalPixelCoordinates( const FVec2I64& iPos )  const { return  iPos / tSuperClass::local_chunk_halfsize_as_pixels; }
    FVec2I64  LocalPixelCoordinatesFromSubChunkCoordinates( const FVec2I64& iPos )  const { return  iPos * tSuperClass::local_chunk_halfsize_as_pixels; }

    uint8 IndexFromSubChunkCoordinates( const FVec2I64& iPos )  const {
        return  static_cast< uint8 >( iPos.y * 2  + iPos.x ); // May Theoretically Overflow, But in context is called with safe values.
    }

    FVec2I64 SubChunkCoordinatesFromIndex( uint8 iIndex )  const {
        return  FVec2I64( iIndex & 0x1, ( iIndex & 0x2 ) >> 1 );
    }

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kQuadree; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        uint8 index = IndexFromSubChunkCoordinates( SubChunkCoordinatesFromLocalPixelCoordinates( iPos ) );
        return  mQuad[index] == nullptr ? iPool->EmptyTile() : mQuad[index]->QueryConstBlockAtPixelCoordinates( iPool, iPos % tSuperClass::local_chunk_halfsize_as_pixels );
    }

    void ReplaceElement( uint8 iIndex, tSubAbstractChunk* iValue ) {
        if( mQuad[iIndex] )
            delete  mQuad[iIndex];
        mQuad[iIndex] = iValue;
    }

    void PerformElementSubdivisionForImminentMutableChangeIfNeeded( uint8 iIndex, tTilePool* iPool ) {
        if( mQuad[iIndex] == nullptr )
            if( tSuperClass::bed ) {
                // X Query Tile, X stands for Refcount decrease needed by caller.
                FTileElement* tile = iPool->XQueryFreshTile();
                mQuad[iIndex] = dynamic_cast< tSubAbstractChunk* >( new tSubDataChunk( tile ) );
                tile->DecreaseRefCount();
            } else {
                mQuad[iIndex] = dynamic_cast< tSubAbstractChunk* >( new tSubQuadtreeChunk() );
            }
        else
            if( mQuad[iIndex]->Type() == eChunkType::kData && !tSuperClass::bed )
                ReplaceElement( iIndex, new tSubQuadtreeChunk( dynamic_cast< tSubDataChunk* >( mQuad[iIndex] )->PointedData() ) );
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        uint8 index = IndexFromSubChunkCoordinates( SubChunkCoordinatesFromLocalPixelCoordinates( iPos ) );
        PerformElementSubdivisionForImminentMutableChangeIfNeeded( index, iPool );
        return  mQuad[index]->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( iPool, iPos % tSuperClass::local_chunk_halfsize_as_pixels );
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size  = static_cast< int >( round( tSuperClass::local_chunk_size_as_pixels * iScale ) );
        auto hsize = static_cast< int >( round( tSuperClass::local_chunk_halfsize_as_pixels * iScale ) );
        DrawRectOutlineNoAA( iDst, default_wireframe_debug_color, FRectI( static_cast< int >( iPos.x ), static_cast< int >( iPos.y ), size, size ) );
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                mQuad[i]->DrawDebugWireframe( iDst, iPos + SubChunkCoordinatesFromIndex( i ) * hsize, iScale );
    }

    virtual  void DrawDebugTileContent( FBlock* iDst, const FVec2I64& iPos ) override {
        auto size  = tSuperClass::local_chunk_size_as_pixels;
        auto hsize = tSuperClass::local_chunk_halfsize_as_pixels;
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                mQuad[i]->DrawDebugTileContent( iDst, iPos + SubChunkCoordinatesFromIndex( i ) * hsize );
    }

    virtual  void SanitizeNow( tTilePool* iPool ) override {
        for( int i = 0; i < 4; ++i ) {
            if( mQuad[i] ) {
                mQuad[i]->SanitizeNow( iPool );
                if( mQuad[i]->Type() == eChunkType::kData ) {
                    // IF after sanitization the child data points to null, simply delete to state it's clear
                    tSubDataChunk* datachunk = dynamic_cast< tSubDataChunk* >( mQuad[i] );
                    if( datachunk->PointedData() == nullptr ) {
                        delete  mQuad[i];
                        mQuad[i] = nullptr;
                    }
                } else {
                    // Else, it's likely a quad.
                    // Check if the quad has the same distributed value and perform merge
                    // if the distributed value is null, just clear delete
                    // else, we can make it a data chunk if it's not bed.
                    tSubQuadtreeChunk* quadchunk = dynamic_cast< tSubQuadtreeChunk* >( mQuad[i] );
                    FTileElement* el;
                    bool uniform = quadchunk->CheckUniformDistributedValue( &el );
                    if( uniform ) {
                        if( el == nullptr ) {
                            delete  mQuad[i];
                            mQuad[i] = nullptr;
                        } else {
                            delete  mQuad[i];
                            mQuad[i] = new tSubDataChunk( el );
                        }
                    }
                }
            }
        }
    }

    virtual  FRectI GetRoughLeafGeometry( const FVec2I64& iPos ) const override {
        FRectI ret;
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                ret = ret.UnionLeaveEmpty( mQuad[i]->GetRoughLeafGeometry( iPos + SubChunkCoordinatesFromIndex( i ) * tSuperClass::local_chunk_halfsize_as_pixels ) );
        return  ret;
    }

    bool CheckUniformDistributedValue( FTileElement** oElem ) {
        // IF all null, return true null
        // IF all data and all same, return true and elem.
        bool all_data = true;
        bool all_null = true;
        bool all_same = true;
        FTileElement* val = nullptr;
        for( int i = 0; i < 4; ++i ) {
            if( mQuad[i] != nullptr ) {
                all_null = false;
                tSubDataChunk* data = dynamic_cast< tSubDataChunk* >( mQuad[i] );
                if( data != nullptr ) {
                    if( i == 0 )
                        val = data->PointedData();
                    else if( val != data->PointedData() )
                        all_same = false;
                }
                else
                {
                    all_data = false;
                }
            } else {
                all_data = false;
            }
        }
        *oElem = val;
        return  all_null || ( all_data && all_same );
    }

private:
    // Private Data Members
    tSubAbstractChunk* mQuad[4];
};

ULIS_NAMESPACE_END

