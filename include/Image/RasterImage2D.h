// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         RasterImage2D.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FRasterImage2D class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Color.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FRasterImage2D
/// @brief      The FRasterImage2D class provides a mean of storing and manipulating
///             digital images in various formats, with direct access to pixel
///             data.
/// @details    FRasterImage2D is the primary class to use when manipulating images
///             with ULIS.
///
///             The block has a format which specifies the type, memory layout,
///             color model, and other informations about the image.
///             The underlying data is always a regular contiguous buffer
///             storage that lives on RAM with no alignment requirement.
///
///             The FRasterImage2D class is very lightweight, the memory load is in the
///             data it points to, and the class never performs any kind of
///             heavy operations. It does not communicate with the GPU or use
///             any kind of hardware accelerated feature just by itself.
///
///             Copy, wether deep or shallow, is explicitely forbidden, If you
///             need to copy image data, you can use the explicit Copy or XCopy
///             functions that ULIS provides.
///
///             FRasterImage2D supports loading an external data pointer and
///             treat it as its own storage: this is useful for sharing memory
///             without having to copy it and you can grab or release ownership
///             at any time with the help of the cleanup callbacks, just make
///             sure to use the proper deallocator when providing external data
///             which was allocated with a custom allocator. However the data
///             is not reference counted.
///
///             You can also provide custom callbacks to signal when the block
///             is dirty, for example use it to upload a small rect of the
///             image to a GPU texture when triggered.
///
///             It is perfectly fine to create FRasterImage2D objects on the stack:
///             \snippet data/block_snippet.h FRasterImage2D on stack
///             But you can also allocate blocks dynamically:
///             \snippet data/block_snippet.h FRasterImage2D on heap
///             It is also sometimes handy to use the CRT safe version if
///             using the \a X functions in your application with dynamic link:
///             \snippet data/block_snippet.h FRasterImage2D X Version
class ULIS_API FRasterImage2D final
    : public IHasFormat
    , public IHasColorSpace
{
public:
    /*! Destroy the block and invoke the cleanup callback. */
    ~FRasterImage2D();

    /*!
    Construct a block with input size and format.

    The underlying bitmap data will be allocated with enough storage space as
    required by the size and format depth. The data is left uninitialized.

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.
    */
    FRasterImage2D(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , const FColorSpace* iColorSpace = nullptr
        , const FOnInvalid& iOnInvalid = FOnInvalid()
        , const FOnCleanup& iOnCleanup = FOnCleanup( &OnCleanup_FreeMemory )
    );

    /*!
    Construct a block from an existing external buffer with input size and
    format.

    The input buffer data is used and no allocation is done here, if coherent
    visual data was present in the buffer it will remain as is. Make sure to
    chose the appropriate size and format, given the buffer length. Bad values
    could result in misinterpreting the visual information or accessing the
    buffer out of bounds and crash or corrupt memory.

    \warning The block will not delete the external input data by default,
    unless you provide and appropriate cleanup callback to handle that. The
    data can be shared with other representations but it must remain valid at
    least as long as the block lifetime.

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.
    */
    FRasterImage2D(
          uint8* iData
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , const FColorSpace* iColorSpace = nullptr
        , const FOnInvalid& iOnInvalid = FOnInvalid()
        , const FOnCleanup& iOnCleanup = FOnCleanup()
    );

    /*!
    Explicitely disabled copy constructor.

    If you need to make a shallow copy with shared image data, use the explicit
    constructor from external data.

    If you need to make a deep copy of the image data, use the explicit Copy or
    XCopy functions that ULIS provides.
    */
    FRasterImage2D( const FRasterImage2D& ) = delete;

    /*!
    Explicitely disabled copy assignment operator.

    If you need to make a shallow copy with shared image data, use the explicit
    constructor from external data.

    If you need to make a deep copy of the image data, use the explicit Copy or
    XCopy functions that ULIS provides.
    */
    FRasterImage2D& operator=( const FRasterImage2D& ) = delete;

    /*!
    Static maker for the FRasterImage2D class

    Construct a block with input size and format.

    In case of dynamic linking, llocate the storage for the block instance in
    the ULIS heap. Use this for convenience when playing around dynamic library
    boundaries or CRT issues.

    Functions or methods marked with a X indicate that an object is allocated
    and that the caller is responsible for deleting it with the appropriate
    deleter function, also marked with a X. In this case, XDelete().

    The buffer data will be allocated with enough storage space as required by
    the size and format depth. The data is left uninitialized.

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.

    \warning Never delete a block with free or with the delete operator unless
    if it was created with XMake()

    \sa XDelete()
    */
    static
    FRasterImage2D* XMake(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , FColorSpace* iColorSpace = nullptr
        , const FOnInvalid& iOnInvalid = FOnInvalid()
        , const FOnCleanup& iOnCleanup = FOnCleanup( &OnCleanup_FreeMemory )
    );

    /*!
    Static maker for the FRasterImage2D class

    Construct a block from an existing external buffer with input size and
    format.

    In case of dynamic linking, allocate the storage for the block instance in
    the ULIS heap. Use this for convenience when playing around dynamic library
    boundaries or CRT issues.

    The input buffer data is used and no allocation is done here, if coherent
    visual data was present in the buffer it will remain as is. Make sure to
    chose the appropriate size and format, given the buffer length. Bad values
    could result in misinterpreting the visual information or accessing the
    buffer out of bounds and crash or corrupt memory.

    \warning The block will not delete the external input data by default,
    unless you provide and appropriate cleanup callback to handle that. The
    data can be shared with other representations but it must remain valid at
    least as long as the block lifetime.

    \warning Functions or methods marked with a X indicate that an object is
    allocated and that the caller is responsible for deleting it with the
    appropriate deleter function, also marked with a X. In this case,
    XDelete().

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.

    \warning Never delete a block with free or with the delete operator unless
    if it was created with XMake()

    \sa XDelete()
    */
    static
    FRasterImage2D* XMake(
          uint8* iData
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , FColorSpace* iColorSpace = nullptr
        , const FOnInvalid& iOnInvalid = FOnInvalid()
        , const FOnCleanup& iOnCleanup = FOnCleanup()
    );

    /*!
    Static deleter for the FRasterImage2D class

    Delete a block that has been previously created using XMake()
    In case of dynamic linking, deallocate the storage for the block instance
    in the ULIS heap. Use this for convenience when playing around dynamic
    library boundaries or CRT issues.

    \warning Never delete a block with XDelete() unless it was created with
    XMake()

    \sa XMake()
    */
    static void XDelete( FRasterImage2D* iBlock );

    /*!
    Obtain a pointer to the raw data at the base element of the underlying
    image buffer.

    This data is the same as the one used by the block. It is non-const and can
    be modified safely, as long as you don't dereference elements outside of
    the buffer boundaries, or trigger race conditions when applying
    multithreaded operations on the same data.

    \sa ScanlineBits()
    \sa PixelBits()
    */
    uint8* Bits();

    /*!
    Obtain a pointer to the raw data at the specified scanline row element of
    the underlying image buffer.

    This data is the same as the one used by the block. It is non-const and can
    be modified safely, as long as you don't dereference elements outside of
    the buffer boundaries, or trigger race conditions when applying
    multithreaded operations on the same data.

    \warning If you specify a row that is negative or bigger than the block
    height, the function will trigger an assert and crash in debug builds, but
    it will fail silently and access the buffer out of bounds in release builds
    , leading to potential memory corruption or crashes further down the line.

    \sa Bits()
    \sa PixelBits()
    */
    uint8* ScanlineBits( uint16 iRow );

    /*!
    Obtain a pointer to the raw data at the specified pixel location of
    the underlying image buffer.

    This data is the same as the one used by the block. It is non-const and can
    be modified safely, as long as you don't dereference elements outside of
    the buffer boundaries, or trigger race conditions when applying
    multithreaded operations on the same data.

    \warning If you specify X and Y coordinates that are either negative or
    bigger than the block width and height respectively, the function will
    trigger an assert and crash in debug builds, but it will fail silently and
    access the buffer out of bounds in release builds, leading to potential
    memory corruption or crashes further down the line.

    \sa Bits()
    \sa ScanlineBits()
    */
    uint8* PixelBits( uint16 iX, uint16 iY );

    /*!
    Obtain a pointer to the raw data at the base element of the underlying
    image buffer.

    This data is the same as the one used by the block. It is const so it
    cannot be modified, but it can be read freely, as long as you don't
    dereference elements outside of the buffer boundaries.

    \sa ScanlineBits()
    \sa PixelBits()
    */
    const uint8* Bits() const;

    /*!
    Obtain a pointer to the raw data at the specified scanline row element of
    the underlying image buffer.

    This data is the same as the one used by the block. It is const so it
    cannot be modified, but it can be read freely, as long as you don't
    dereference elements outside of the buffer boundaries.

    \warning If you specify a row that is negative or bigger than the block
    height, the function will trigger an assert and crash in debug builds, but
    it will fail silently and access the buffer out of bounds in release
    builds, leading to potential memory corruption or crashes further down the
    line.

    \sa Bits()
    \sa PixelBits()
    */
    const uint8* ScanlineBits( uint16 iRow ) const;

    /*!
    Obtain a pointer to the raw data at the specified pixel location of
    the underlying image buffer.

    This data is the same as the one used by the block. It is const so it
    cannot be modified, but it can be read freely, as long as you don't
    dereference elements outside of the buffer boundaries.

    \warning If you specify X and Y coordinates that are either negative or
    bigger than the block width and height respectively, the function will
    trigger an assert and crash in debug builds, but it will fail silently and
    access the buffer out of bounds in release builds, leading to potential
    memory corruption or crashes further down the line.

    \sa Bits()
    \sa ScanlineBits()
    */
    const uint8* PixelBits( uint16 iX, uint16 iY ) const;

    /*!
    Return the width of the block.

    \sa Height()
    \sa Area()
    \sa Rect()
    */
    uint16 Width() const;

    /*!
    Return the height of the block.

    \sa Width()
    \sa Area()
    \sa Rect()
    */
    uint16 Height() const;

    /*!
    Return the area of the block.

    This can also be interpreted as the full length of the underlying buffer.

    \sa Width()
    \sa Height()
    \sa Rect()
    */
    uint32 Area() const;

    /*!
    Return a rect representing the bounding box of the block.

    \sa Width()
    \sa Height()
    \sa Area()
    */
    FRectI Rect() const;

    /*!
    Return the numbers of bytes per scanline

    \sa BytesTotal()
    */
    uint32 BytesPerScanLine() const;

    /*!
    Return the total numbers of bytes in the block.

    \sa BytesPerScanLine()
    */
    uint64 BytesTotal() const;

    /*!
    Dirty the entire block and trigger the invalid callback if set.

    The optional \a iCallOnInvalid parameter can be set to false in order to
    conditionaly disable the callback trigger depending on your needs.

    In the case of many successive calls to dirty, it can be more efficient to
    just disable them first and call it only once later.
    */
    void Dirty( bool iCallOnInvalid = true ) const;

    /*!
    Dirty a part of the block according to the input rect and trigger the
    invalid callback if set.

    The optional \a iCallOnInvalid parameter can be set to false in order to
    conditionaly disable the callback trigger depending on your needs.

    In the case of many successive calls to dirty with different small
    geometries, it can be more efficient to just disable them first and call it
    only once later with the union of all invalid rects.
    */
    void Dirty( const FRectI& iRect, bool iCallOnInvalid = true ) const;

    /*!
    Return the color of the pixel at the given coordinates.

    The FColor is a copy of the pixel data, safe to read and write without
    altering the block contents.

    \warning If you specify X and Y coordinates that are either negative or
    bigger than the block width and height respectively, the function will
    trigger an assert and crash in debug builds, but it will fail silently and
    access the buffer out of bounds in release builds, leading to potential
    memory corruption or crashes further down the line.

    \sa Pixel()
    \sa PixelBits()
    \sa FColor
    */
    FColor Color( uint16 iX, uint16 iY ) const;

    /*!
    Return the pixel at the given coordinates.

    The FPixel is an implicitely shared representation of the block data at the
    input pixel location, if you modify it you will alter the contents of the
    block. It is sometimes more convenient than PixelBits() in order to read
    or modify the values, and does not perform a copy.

    \warning If you specify X and Y coordinates that are either negative or
    bigger than the block width and height respectively, the function will
    trigger an assert and crash in debug builds, but it will fail silently and
    access the buffer out of bounds in release builds, leading to potential
    memory corruption or crashes further down the line.

    \sa Color()
    \sa PixelBits()
    \sa FPixel
    */
    FPixel Pixel( uint16 iX, uint16 iY );

    /*!
    Return the const pixel at the given coordinates.

    The FPixel is an implicitely shared representation of the block data at the
    input pixel location It is sometimes more convenient than PixelBits() in
    order to read the values, and does not perform a copy.

    \warning If you specify X and Y coordinates that are either negative or
    bigger than the block width and height respectively, the function will
    trigger an assert and crash in debug builds, but it will fail silently and
    access the buffer out of bounds in release builds, leading to potential
    memory corruption or crashes further down the line.

    \sa Color()
    \sa PixelBits()
    \sa FPixel
    */
    const FPixel Pixel( uint16 iX, uint16 iY ) const;

    /*!
    Set a new invalid callback that will be called on dirty.

    \sa OnCleanup()
    */
    void OnInvalid( const FOnInvalid& iOnInvalid );

    /*!
    Set a new cleanup callback that will be called on destruction.

    \sa OnInvalid()
    */
    void OnCleanup( const FOnCleanup& iOnCleanup );

    /*!
    Reconstruct the internal representation from an existing external buffer
    with input size and format.

    \sa FRasterImage2D()
    */
    void ReloadFromData(
          uint8* iData
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , const FColorSpace* iColorSpace = nullptr
        , const FOnInvalid& iOnInvalid = FOnInvalid()
        , const FOnCleanup& iOnCleanup = FOnCleanup()
    );

private:
    uint8* mBitmap; ///< Contiguous memory storage buffer for the block.
    uint16 mWidth; ///< The width of the block.
    uint16 mHeight; ///< Height of the block.
    uint32 mBytesPerScanline; ///< Cached number of bytes per scanline.
    uint64 mBytesTotal; ///< Cached number of bytes for the whole buffer.
    FOnInvalid mOnInvalid; ///< The callback for when the block is destroyed.
    FOnCleanup mOnCleanup; ///< The callback for when the block is dirty.
};

ULIS_NAMESPACE_END

