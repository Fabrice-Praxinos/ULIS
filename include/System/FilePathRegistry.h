// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FilePathRegistry.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FFilePathRegistry class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <list>
#include <unordered_map>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFilePathRegistry
/// @brief      The FFilePathRegistry class provides a mean of storing and manipulating file paths registries with custom filters.
class ULIS_API FFilePathRegistry
{
    typedef std::string  tName;
    typedef std::string  tPath;
    typedef std::list< std::string >  tStringList;
    typedef std::unordered_map< tName, tPath >  tRecords;

public:
    // Construction / Destruction
    virtual ~FFilePathRegistry();
    FFilePathRegistry();

public:
    // Public API
    const tStringList& LookupPaths() const;
    const tStringList& Filters() const;
    const tRecords&  Records() const;
    void AddLookupPath( const std::string& iPath );
    void AddLookupPaths( const tStringList& iPaths );
    void AddFilter( const std::string& iFilter );
    void AddFilters( const tStringList& iFilters );
    void Parse();
    const std::string FilePathForExactMatchingName( const std::string& iName ) const;
    const std::string FilePathForClosestMatchingName( const std::string& iName ) const;

private:
    // Private API
    virtual std::string MakeName( const std::string& iFile ) const;

private:
    // Private Data Members
    tStringList mLookupPaths;
    tStringList mFilters;
    tRecords mRecords;
};

ULIS_NAMESPACE_END

