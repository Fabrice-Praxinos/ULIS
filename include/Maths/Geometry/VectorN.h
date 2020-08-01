// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         VectorN.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TVectorN class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry/VectorFunc.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TVectorN
/// @brief      The TVectorN class provides a simple ND vector class for vector
///             geometry applications.
/// @details    The TVectorN class is a template class that provides basic ND
///             vector functionnalities, it can be used with any numeric types,
///             ranging from integer values to floating point values.
template< typename T, typename P, uint8 N >
class TVectorN
{
    // Typedefs
    typedef P tComputation;

    // Members
    /*! The components of the ND vector. */
    T m[N];

public:
    // Constructors
    /*! Default constructor for the vector, with default zero values. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >();

    /*! Construct the vector from input value all components will have said value */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >( T iValue );

    /*! Construct the vector from input values. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >( std::initializer_list< T > iValues );



    // Conversion
    /*! Construct the vector from input vector of any convertible type. */
    template< typename U, typename Q, uint8 M >
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >( const TVectorN< U, Q, M >& iOther );



    // Named Functions
    /*! Return the euclidean distance of the vector. */
    ULIS3_VECTOR_FUNC tComputation Distance() const;

    /*!
    Return the squared euclidean distance of the vector.
    This can be useful for testing against another squared distance, thus
    saving a square root calculation.
    */
    ULIS3_VECTOR_FUNC tComputation DistanceSquared() const;

    /*! Return the manhattan distance of the vector. */
    ULIS3_VECTOR_FUNC tComputation ManhattanDistance() const;

    /*! Dot Product */
    ULIS3_VECTOR_FUNC tComputation DotProduct( const TVectorN< T, P, N >& iOther ) const;

    /*! Normalize this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& Normalize();

    /*! Return the normalized version of this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N > Normalized() const;



    // Comparison operators
    /*! Compare this vector to the other vector for strict equality. */
    ULIS3_VECTOR_FUNC bool operator==( const TVectorN< T, P, N >& iOther ) const;

    /*! Compare this vector to the other vector for strict inequality. */
    ULIS3_VECTOR_FUNC bool operator!=( const TVectorN< T, P, N >& iOther ) const;



    // Unary Negative Operator
    /*! Unary Negative Operator */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N > operator-() const;



    // Unary Operators with scalar
    /*! Add the input value to this vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator+=( T iValue );

    /*! Substract the input value to this vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator-=( T iValue );

    /*! Multiply this vector by the input value and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator*=( T iValue );

    /*! Divide this vector by the input value and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator/=( T iValue );

    /*! Compute modulo of this vector by the input value and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator%=( T iValue );



    // Unary Operators with vector
    /*! Add the input to this vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator+=(const TVectorN< T, P, N >& iOther );

    /*! Substract the input to this vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator-=(const TVectorN< T, P, N >& iOther );

    /*! Multiply this vector by the input vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator*=(const TVectorN< T, P, N >& iOther );

    /*! Divide this vector by the input vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator/=(const TVectorN< T, P, N >& iOther );

    /*! Compute modulo of this vector by the input vector and return a reference to this vector. */
    ULIS3_VECTOR_FUNC TVectorN< T, P, N >& operator%=(const TVectorN< T, P, N >& iOther );



    // Other Operators
    /*! Access component x or y by index, return a reference to the component. */
    ULIS3_VECTOR_FUNC T& operator[]( int iIndex );

    /*! Access component x or y by index, return a const reference to the component. */
    ULIS3_VECTOR_FUNC const T& operator[]( int iIndex ) const;
};



// Constructors
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >::TVectorN()
{
    for( uint8 i = 0; i < N; ++i )
        m[i] = static_cast< T >( 0 );
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >::TVectorN( T iValue )
{
    for( uint8 i = 0; i < N; ++i )
        m[i] = iValue;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >::TVectorN( std::initializer_list< T > iValues )
{
    for( uint8 i = 0; i < iValues.size(); ++i )
        m[i] = *( iValues.begin() + i );

    for( uint8 i = iValues.size(); i < N; ++i )
        m[i] = static_cast< T >( 0 );
}



// Conversion
template< typename T, typename P, uint8 N >
template< typename U, typename Q, uint8 M >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >::TVectorN( const TVectorN< U, Q, M >& iOther )
{
    const uint8 min = FMaths::Min( N, M );
    const uint8 max = FMaths::Max( N, M );

    for( uint8 i = 0; i < len; ++i )
        m[i] = static_cast< T >( iOther.m[i] );

    for( uint8 i = min; i < max; ++i )
        m[i] = static_cast< T >( 0 );
}



// Named Functions
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC
TVectorN< T, P, N >::tComputation
TVectorN< T, P, N >:: Distance() const {
    return  FMaths::Sqrt( DistanceSquared() );
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC
TVectorN< T, P, N >::tComputation
TVectorN< T, P, N >::DistanceSquared() const {
    tComputation res = 0;

    for( uint8 i = 0; i < N; ++i )
        res += static_cast< tComputation >(
               static_cast< tComputation >( m[i] )
             * static_cast< tComputation >( m[i] )
            );

    return  res;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC
TVectorN< T, P, N >::tComputation
TVectorN< T, P, N >::ManhattanDistance() const {
    tComputation res = 0;

    for( uint8 i = 0; i < N; ++i )
        res += FMaths::Abs( static_cast< tComputation >( m[i] ) );

    return  res;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC
TVectorN< T, P, N >::tComputation
TVectorN< T, P, N >::DotProduct( const TVectorN& iOther ) const {
    tComputation res = 0;

    for( uint8 i = 0; i < N; ++i )
        res += static_cast< tComputation >(
               static_cast< tComputation >( m[i] )
             * static_cast< tComputation >( iOther.m[i] )
            );

    return  res;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::Normalize() {
    tComputation inv_distance = FMaths::InvSqrt( DistanceSquared() );

    for( uint8 i = 0; i < N; ++i )
        m[i] = static_cast< T >( m[i] * inv_distance );

    return  *this
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N > TVectorN< T, P, N >::Normalized() const {
    TVectorN< T, P, N > result = *this;

    result.Normalize();

    return  result;
}



// Comparison operators
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC bool TVectorN< T, P, N >::operator==( const TVectorN< T, P, N >& iOther ) const {
    for( int i = 0; i < N; ++i )
        if( m[i] != iOther.m[i] )
            return  false;

    return  true;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC bool TVectorN< T, P, N >::operator!=(const TVectorN< T, P, N >& iOther ) const {
    return  !( *this == iOther );
}



// Unary negative operator
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N > TVectorN< T, P, N >::operator-() const {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result[i] = -m[i];

    return  result;
}



// Unary Operators with scalar
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator+=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] += iValue;

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator-=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] -= iValue;

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator*=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] *= iValue;

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator/=( T iValue ) {
    ULIS3_ASSERT( iValue != static_cast< T >( 0 ), "Division by zero" );

    for( int i = 0; i < N; ++i )
        m[i] /= iValue;

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator%=( T iValue ) {
    ULIS3_ASSERT( iValue != static_cast< T >( 0 ), "Division by zero" );

    for( int i = 0; i < N; ++i )
        m[i] = FMaths::Mod( m[i], iValue );

    return  *this;
}



// Unary Operators with vectors
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator+=(const TVectorN< T, P, N >& iOther ) {
    for( int i = 0; i < N; ++i )
        m[i] += iOther.m[i];

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator-=(const TVectorN< T, P, N >& iOther ) {
    for( int i = 0; i < N; ++i )
        m[i] -= iOther.m[i];

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator*=(const TVectorN< T, P, N >& iOther ) { 
    for( int i = 0; i < N; ++i )
        m[i] *= iOther.m[i];

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator/=(const TVectorN& iOther ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iOther.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    for( int i = 0; i < N; ++i )
        m[i] /= iOther.m[i];

    return  *this;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC TVectorN< T, P, N >& TVectorN< T, P, N >::operator%=(const TVectorN& iOther ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iOther.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    for( int i = 0; i < N; ++i )
        m[i] = FMaths::Mod( m[i], iOther.m[i] );

    return  *this;
}



// Other Operators
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T& TVectorN< T, P, N >::operator[]( int iIndex ) {
    ULIS3_ASSERT( iIndex >= 0 && iIndex < N, "Bad Index" );
    return  m[ iIndex ];
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC const T& TVectorN< T, P, N >::operator[]( int iIndex ) const {
    ULIS3_ASSERT( iIndex >= 0 && iIndex < N, "Bad Index" );
    return  m[ iIndex ];
}





// Binary Operators with scalar on left hand side
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator+( T iValue, const TVectorN< T, P, N >& iVector ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue + iVector.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator-( T iValue, const TVectorN< T, P, N >& iVector ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue - iVector.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator*( T iValue, const TVectorN< T, P, N >& iVector ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue * iVector.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator/( T iValue, const TVectorN< T, P, N >& iVector ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iVector.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue / iVector.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator%( T iValue, const TVectorN< T, P, N >& iVector ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iVector.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = FMaths::Mod( iValue, iVector.m[i] );

    return  result;
}



// Binary Operators with scalar on right hand side
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator+( const TVectorN< T, P, N >& iVector, T iValue ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] + iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator-( const TVectorN< T, P, N >& iVector, T iValue ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] - iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator*( const TVectorN< T, P, N >& iVector, T iValue ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] * iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator/( const TVectorN< T, P, N >& iVector, T iValue ) {
    ULIS3_ASSERT( iValue != static_cast< T >( 0 ), "Division by zero" );
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] / iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator%( const TVectorN< T, P, N >& iVector, T iValue ) {
    ULIS3_ASSERT( iValue != static_cast< T >( 0 ), "Division by zero" );
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = FMaths::Mod( iVector.m[i], iValue );

    return  result;
}



// Binary Operators with vector
template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator+( const TVectorN< T, P, N >& iVector, const TVectorN< T, P, N >& iOther ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] + iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator-( const TVectorN< T, P, N >& iVector, const TVectorN< T, P, N >& iOther ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] - iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator*( const TVectorN< T, P, N >& iVector, const TVectorN< T, P, N >& iOther ) {
    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] * iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator/( const TVectorN< T, P, N >& iVector, const TVectorN< T, P, N >& iOther ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iOther.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iVector.m[i] / iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS3_VECTOR_FUNC T operator%( const TVectorN< T, P, N >& iVector, const TVectorN< T, P, N >& iOther ) {
#ifdef ULIS3_ASSERT_ENABLED
    for( int i = 0; i < N; ++i )
        ULIS3_ASSERT( iOther.m[i] != static_cast< T >( 0 ), "Division by zero" );
#endif // ULIS3_ASSERT_ENABLED

    TVectorN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = FMaths::Mod( iVector.m[i], iOther.m[i] );

    return  result;
}

ULIS3_NAMESPACE_END

