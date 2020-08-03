// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         MatrixN.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TMatrixN class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/MatrixFunc.h"
#include "Math/Geometry/VectorN.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TMatrixN
/// @brief      The TMatrixN class provides a simple N square matrix class for
///             geometry applications.
/// @details    The TMatrixN class is a template class that provides basic matrix
///             functionnalities, it can be used with any numeric types,
///             ranging from integer values to floating point values.
template< typename T, typename P, uint8 N >
class TMatrixN
{
    // Typedefs
    typedef P tComputation;
    typedef TVectorN< T, P, N > tRow;
    typedef TVectorN< T, P, N > tColumn;

    // Members
    /*! The N rows of the N matrix. */
    tRow m[N];

public:
    // Constructors
    /*! Default constructor ( identity matrix ). */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >();

    /*! Constructor from input diagonal value. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >( T iValue );

    /*! Constructor from input cells values. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >( std::initializer_list< T > iValues );

    /*! Constructor from row vector values. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >( std::initializer_list< tRow > iRows );

    /*! Copy constructor. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >( const TMatrixN< T, P, N >& iOther );

    /*! Assignment operator from matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator=( const TMatrixN< T, P, N >& iOther );



    // Conversion
    /*! Conversion constructor from other matrix type. */
    template< typename U, typename Q, uint8 M >
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >( const TMatrixN< U, Q, M >& iOther );



    // Named Functions
    /*! Return a pointer to the base storage. */
    ULIS_VECTOR_FUNC T* Bits();

    /*! Return a pointer to the base storage. */
    ULIS_VECTOR_FUNC const T* Bits() const;

    /*! Obtain the inverse of the matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N > Inverse();

    /*! Obtain the transpose of the matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N > Transpose();

    /*! Obtain the transpose of the matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N > Rotated90CCW();

    /*! Obtain the determinant of the matrix. */
    ULIS_MATRIX_FUNC tComputation Determinant();



    // Boolean operators
    /*! Equality operator. */
    ULIS_MATRIX_FUNC bool operator==( const TMatrixN< T, P, N >& iOther );

    /*! Inequality operator. */
    ULIS_MATRIX_FUNC bool operator!=( const TMatrixN< T, P, N >& iOther );



    // Unary Negative Operator
    /*! Unary negative operator. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator-() const;



    // Unary arithmetic operator with scalar
    /*! Unary addition operator with value. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator+=( T iValue );

    /*! Unary substraction operator with value. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator-=( T iValue );

    /*! Unary multiplication operator with value. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator*=( T iValue );

    /*! Unary division operator with value. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator/=( T iValue );



    // Unary arithmetic operator with matrix
    /*! Unary addition operator with matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator+=( const TMatrixN< T, P, N >& iOther );

    /*! Unary substraction operator with matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator-=( const TMatrixN< T, P, N >& iOther );

    /*! Unary multiplication operator with matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator*=( const TMatrixN< T, P, N >& iOther );

    /*! Unary division operator with matrix. */
    ULIS_MATRIX_FUNC TMatrixN< T, P, N >& operator/=( const TMatrixN< T, P, N >& iOther );



    // Other Operators
    /*! Subscript accessor for matrix columns. */
    ULIS_MATRIX_FUNC tRow& operator[]( int iIndex );

    /*! Subscript accessor for const matrix columns. */
    ULIS_MATRIX_FUNC const tRow& operator[]( int iIndex ) const;
};





// detail
namespace detail {
/*!
    LUP Decomposition implementation
    @param ioMat, the matrix of size N to decompose, it is modified to contain L-E and U.
    @param oPermut, permutation vector of size N+1.
    @return boolean stating if the decomposition succeeded or not ( degenerate ).
*/
template< typename T, typename P, uint8 N >
bool PartialPivotingLUDecomposition_imp(
      TMatrixN< T, P, N >& ioMat
    , TVectorN< int, float, N + 1 >& oPermut
)
{
    int i, j, k, imax;
    T maxMat, absMat;
    TVectorN< T, P, N > temp_row;

    for( i = 0; i <= N; ++i )
        oPermut[i] = i; // Unit permutation matrix, oPermut[N] initialized with N

    for( i = 0; i < N; ++i ) {
        maxMat = static_cast< T >( 0 );
        imax = i;

        for( k = i; k < N; ++k ) {
            absMat = FMath::Abs( ioMat[k][i] )
            if( absMat > maxMat) {
                maxMat = absMat;
                imax = k;
            }
        }

        if( maxMat < FMath::kEpsilonf )
            return  false; // error, degenerate

        if( imax != i ) {
            //pivoting P
            j = oPermut[i];
            oPermut[i] = oPermut[imax];
            oPermut[imax] = j;

            // swapping rows of ioMat
            temp_row = ioMat[i];
            ioMat[i] = ioMat[imax];
            ioMat[imax] = temp_row;

            // counting pivots starting from N (for determinant)
            oPermut[N]++;
        }

        for (j = i + 1; j < N; j++) {
            ioMat[j][i] /= ioMat[i][i];

            for (k = i + 1; k < N; k++)
                ioMat[j][k] -= ioMat[j][i] * ioMat[i][k];
        }
    }

    return  true; // success
}

/*!
    LUP Solving implementation
    @param iMat, filled from PartialPivotingLUDecomposition_imp
    @param iPermut, filled from PartialPivotingLUDecomposition_imp
    @param iVec, right hand side vector
    @return solution vector "x" of ioMat * x = iVec.
*/
template< typename T, typename P, uint8 N >
TVectorN< T, P, N >
PartialPivotingLUSolve_imp(
      const TMatrixN< T, P, N >& iMat
    , const TVectorN< int, float, N + 1 >& iPermut
    , const TVectorN< T, P, N >& iVec
)
{
    TVectorN< T, P, N > result;
    for( int i = 0; i < N; ++i ) {
        result[i] = iVec[ iPermut[i] ];

        for( int k = 0; k < i; ++k )
            result[i] -= iMat[i][k] * result[k];
    }

    for( int i = N - 1; i >= 0; --i ) {
        for( int k = i + 1; k < N; ++k )
            result[i] -= iMat[i][k] * result[k];

        result[i] = result[i] / iMat[i][i];
    }

    return  result;
}

/*!
    LUP Invert implementation
    @param iMat, filled from PartialPivotingLUDecomposition_imp
    @param iPermut, filled from PartialPivotingLUDecomposition_imp
    @return Inverse of iMat.
*/
template< typename T, typename P, uint8 N >
TMatrixN< T, P, N >
PartialPivotingLUInvert_imp( 
      const TMatrixN< T, P, N >& iMat
    , const TVectorN< int, float, N + 1 >& iPermut
)
{
    TMatrixN< T, P, N > result;

    for( int j = 0; j < N; ++j ) {
        for( int i = 0; i < N; ++i ) {
            if( iPermut[i] == j )
                result[i][j] = static_cast< T >( 1 );
            else
                result[i][j] = static_cast< T >( 0 );

            for( int k = 0; k < i; ++k )
                result[i][j] -= iMat[i][k] * result[k][j];
        }

        for( int i = N - 1; i >= 0; --i ) {
            for( int k = i + 1; k < N; ++k )
                result[i][j] -= iMat[i][k] * result[k][j];

            result[i][j] = result[i][j] / iMat[i][i];
        }
    }

    return  result;
}

/*!
    LUP Invert implementation
    @param iMat, filled from PartialPivotingLUDecomposition_imp
    @param iPermut, filled from PartialPivotingLUDecomposition_imp
    @return Determinant of iMat.
*/
template< typename T, typename P, uint8 N >
TMatrixN< T, P, N >
PartialPivotingLUDeterminant_imp( 
      const TMatrixN< T, P, N >& iMat
    , const TVectorN< int, float, N + 1 >& iPermut
)
{
    T det = iMat[0][0];

    for( int i = 1; i < N; ++i )
        det *= iMat[i][i];

    if( ( iPermut[N] - N) % 2 == 0 )
        return  det;
    else
        return  -det;
}

} // namespace detail





// Constructors
template< typename T, typename P, uint8 N >
ULIS_VECTOR_FUNC TMatrixN< T, P, N >::TMatrixN()
{
    for( uint8 i = 0; i < N; ++i )
        m[i] = tRow();
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::TMatrixN( T iValue )
{
    for( uint8 i = 0; i < N; ++i ) {
        m[i] = tRow();
        m[i][i] = iValue;
    }
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::TMatrixN( std::initializer_list< T > iValues )
{
    const uint16 max = N * N;
    for( uint8 i = 0; i < iValues.size(); ++i ) {
        uint8 x = i % N;
        uint8 y = i / N;
        m[y][x] = *( iValues.begin() + i );
    }

    for( uint8 i = iValues.size(); i < max; ++i ) {
        uint8 x = i % N;
        uint8 y = i / N;
        m[y][x] = 0;
    }
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::TMatrixN( std::initializer_list< tRow > iRows )
{
    for( uint8 i = 0; i < iRows.size(); ++i )
        m[i] = *( iRows.begin() + i );

    for( uint8 i = iValues.size(); i < N; ++i )
        m[i] = tRow();
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::TMatrixN( const TMatrixN< T, P, N >& iOther )
{
    for( uint8 i = 0; i < N; ++i )
        m[i] = iOther.m[i];
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator=( const TMatrixN< T, P, N >& iOther ) {
    for( uint8 i = 0; i < N; ++i )
        m[i] = iOther.m[i];
}



// Conversion
template< typename T, typename P, uint8 N >
template< typename U, typename Q, uint8 M >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::TMatrixN( const TMatrixN< U, Q, M >& iOther )
{
    const uint8 min = FMath::Min( N, M );
    const uint8 max = FMath::Max( N, M );

    for( uint8 i = 0; i < min; ++i )
        m[i] = iOther.m[i];

    for( uint8 i = min; i < max; ++i )
        m[i] = tRow();
}



// Named Functions
template< typename T, typename P, uint8 N >
ULIS_VECTOR_FUNC
T*
TMatrixN< T, P, N >::Bits() {
    return  &(m[0].m[0])
}

template< typename T, typename P, uint8 N >
ULIS_VECTOR_FUNC
const T*
TMatrixN< T, P, N >::Bits() const {
    return  &(m[0].m[0])
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC
TMatrixN< T, P, N >
TMatrixN< T, P, N >::Inverse() {
    TMatrixN< T, P, N > LU = (*this);
    TVectorN< int, float, N + 1 > permut;
    detail::PartialPivotingLUDecomposition_imp( LU, permut );
    return  detail::PartialPivotingLUInvert_imp( LU, permut );
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC
TMatrixN< T, P, N >
TMatrixN< T, P, N >::Transpose() {
    TMatrixN< T, P, N > result;

    for( uint8 y = 0; y < N; ++y )
        for( uint8 x = 0; x < N; ++x )
            result.m[y][x] = m[ x ][ y ];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC
TMatrixN< T, P, N >
TMatrixN< T, P, N >::Rotated90CCW() {
    TMatrixN< T, P, N > result;

    for( uint8 y = 0; y < N; ++y )
        for( uint8 x = 0; x < N; ++x )
            result.m[y][x] = m[ x ][ N - y ];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC
TMatrixN< T, P, N >::tComputation
TMatrixN< T, P, N >::Determinant() {
    TMatrixN< T, P, N > LU = (*this);
    TVectorN< int, float, N + 1 > permut;
    detail::PartialPivotingLUDecomposition_imp( LU, permut );
    return  detail::PartialPivotingLUDeterminant_imp( LU, permut );
}



// Boolean operators
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC bool TMatrixN< T, P, N >::operator==( const TMatrixN< T, P, N >& iOther ) {
    for( int i = 0; i < N; ++i )
        if( m[i] != iOther.m[i] )
            return  false;

    return  true;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC bool TMatrixN< T, P, N >::operator!=( const TMatrixN< T, P, N >& iOther ) {
    return  !( *this == iOther );
}



// Unary Negative Operator
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > TMatrixN< T, P, N >::operator-() const {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result[i] = -m[i];

    return  result;
}



// Unary arithmetic operator with scalar
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator+=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] += iValue;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator-=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] -= iValue;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator*=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] *= iValue;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator/=( T iValue ) {
    for( int i = 0; i < N; ++i )
        m[i] /= iValue;
}



// Unary arithmetic operator with matrix
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator+=( const TMatrixN& iOther ) {
    for( int i = 0; i < N; ++i )
        m[i] += iOther.m[i];
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator-=( const TMatrixN& iOther ) {
    for( int i = 0; i < N; ++i )
        m[i] -= iOther.m[i];
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator*=( const TMatrixN& iOther ) {
    return  ( *this = *this * iOther );
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >& TMatrixN< T, P, N >::operator/=( const TMatrixN& iOther ) {
    return  ( *this *= iOther.Inverse() );
}



// Other Operators
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::tColumn& TMatrixN< T, P, N >::operator[]( int iIndex ) {
    ULIS_ASSERT( iIndex >= 0 && iIndex < N, "Bad Index" );
    return  m[ iIndex ];
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC const TMatrixN< T, P, N >::tColumn& TMatrixN< T, P, N >::operator[]( int iIndex ) const {
    ULIS_ASSERT( iIndex >= 0 && iIndex < N, "Bad Index" );
    return  m[ iIndex ];
}



// Binary arithmetic operator with scalar on left hand side
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator+( T iValue, const TMatrixN< T, P, N >& iMat ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue + iMat.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator-( T iValue, const TMatrixN< T, P, N >& iMat ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue - iMat.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator*( T iValue, const TMatrixN< T, P, N >& iMat ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue * iMat.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator/( T iValue, const TMatrixN< T, P, N >& iMat ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iValue / iMat.m[i];

    return  result;
}



// Binary arithmetic operator with scalar on right hand side
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator+( const TMatrixN< T, P, N >& iMat, T iValue ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] + iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator-( const TMatrixN< T, P, N >& iMat, T iValue ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] - iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator*( const TMatrixN< T, P, N >& iMat, T iValue ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] * iValue;

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator/( const TMatrixN< T, P, N >& iMat, T iValue ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] / iValue;

    return  result;
}



// Binary arithmetic operator with other matrix
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator+( const TMatrixN< T, P, N >& iMat, const TMatrixN< T, P, N >& iOther ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] + iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator-( const TMatrixN< T, P, N >& iMat, const TMatrixN< T, P, N >& iOther ) {
    TMatrixN< T, P, N > result;

    for( int i = 0; i < N; ++i )
        result.m[i] = iMat.m[i] - iOther.m[i];

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator*( const TMatrixN< T, P, N >& iMat, const TMatrixN< T, P, N >& iOther ) {
    TMatrixN< T, P, N > result;
    TMatrixN< T, P, N > rotated = iOther.Rotated90CCW();

    for( uint8 y = 0; y < N; ++y )
        for( uint8 x = 0; x < N; ++x )
            result.m[y][x] = ( iMat.m[y] * rotated[ N - x ] ).Sum();

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N > operator/( const TMatrixN< T, P, N >& iMat, const TMatrixN< T, P, N >& iOther ) {
    return  iMat * iOther.Inverse();
}



// Binary matrix vector multiplication operators.
template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::tColumn operator*( const TMatrixN< T, P, N >& iMat, const TMatrixN< T, P, N >::tRow& iRow ) {
    tColumn result;

    for( uint8 y = 0; y < N; ++y )
        result.m[y] = ( iMat.m[y] * iRow ).Sum();

    return  result;
}

template< typename T, typename P, uint8 N >
ULIS_MATRIX_FUNC TMatrixN< T, P, N >::tRow operator*( const TMatrixN< T, P, N >::tColumn& iCol, const TMatrixN< T, P, N >& iMat ) {
    tRow result;
    TMatrixN< T, P, N > rotated = iMat.Rotated90CCW();

    for( uint8 y = 0; y < N; ++y )
        result.m[y] = ( rotated.m[ N - y ] * iCol ).Sum();
}

ULIS_NAMESPACE_END
