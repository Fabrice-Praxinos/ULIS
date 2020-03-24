// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Tile.cpp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Tile class.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/Sparse/Tile.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// FTileElement
FTileElement::~FTileElement()
{}

FTileElement::FTileElement()
    : mBlock( nullptr )
    , mHash( 0 )
    , mDirty( false )
    , mRefCount( 0 )
{}

FTileElement::FTileElement( FBlock* iPtr )
    : mBlock( iPtr )
    , mHash( 0 )
    , mDirty( true )
    , mRefCount( 0 )
{}

void FTileElement::DecreaseRefCount() { mRefCount--; };
void FTileElement::DecreaseRefCount( int iVal ) { mRefCount-= iVal; };
void FTileElement::IncreaseRefCount() { mRefCount++; };
void FTileElement::IncreaseRefCount( int iVal ) { mRefCount+= iVal; };

ULIS2_NAMESPACE_END
