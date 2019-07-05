// Copyright 2018-2019 Praxinos, Inc. All Rights Reserved.

#include "ULIS/Data/ULIS.Data.MD5.h"

/////////////////////////////////////////////////////
// Utility Macro


#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21


namespace ULIS {


/////////////////////////////////////////////////////
// FMD5
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- Construction


FMD5::FMD5()
{
    Init();
}


FMD5::FMD5( const std::string &text )
{
    Init();
    Update( text.c_str(), text.length() );
    Finalize();
}


FMD5::FMD5( const uint8* data, int length )
{
    Init();
    Update( data, length );
    Finalize();
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Private Internals


void
FMD5::Init()
{
    finalized=false;
    count[0] = 0;
    count[1] = 0;
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}


void
FMD5::Transform( const unsigned char block[64] )
{
    uint32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    Decode( x, block, 64 );

    FF( a, b, c, d, x[ 0], S11, 0xd76aa478 );
    FF( d, a, b, c, x[ 1], S12, 0xe8c7b756 );
    FF( c, d, a, b, x[ 2], S13, 0x242070db );
    FF( b, c, d, a, x[ 3], S14, 0xc1bdceee );
    FF( a, b, c, d, x[ 4], S11, 0xf57c0faf );
    FF( d, a, b, c, x[ 5], S12, 0x4787c62a );
    FF( c, d, a, b, x[ 6], S13, 0xa8304613 );
    FF( b, c, d, a, x[ 7], S14, 0xfd469501 );
    FF( a, b, c, d, x[ 8], S11, 0x698098d8 );
    FF( d, a, b, c, x[ 9], S12, 0x8b44f7af );
    FF( c, d, a, b, x[10], S13, 0xffff5bb1 );
    FF( b, c, d, a, x[11], S14, 0x895cd7be );
    FF( a, b, c, d, x[12], S11, 0x6b901122 );
    FF( d, a, b, c, x[13], S12, 0xfd987193 );
    FF( c, d, a, b, x[14], S13, 0xa679438e );
    FF( b, c, d, a, x[15], S14, 0x49b40821 );
    GG( a, b, c, d, x[ 1], S21, 0xf61e2562 );
    GG( d, a, b, c, x[ 6], S22, 0xc040b340 );
    GG( c, d, a, b, x[11], S23, 0x265e5a51 );
    GG( b, c, d, a, x[ 0], S24, 0xe9b6c7aa );
    GG( a, b, c, d, x[ 5], S21, 0xd62f105d );
    GG( d, a, b, c, x[10], S22,  0x2441453 );
    GG( c, d, a, b, x[15], S23, 0xd8a1e681 );
    GG( b, c, d, a, x[ 4], S24, 0xe7d3fbc8 );
    GG( a, b, c, d, x[ 9], S21, 0x21e1cde6 );
    GG( d, a, b, c, x[14], S22, 0xc33707d6 );
    GG( c, d, a, b, x[ 3], S23, 0xf4d50d87 );
    GG( b, c, d, a, x[ 8], S24, 0x455a14ed );
    GG( a, b, c, d, x[13], S21, 0xa9e3e905 );
    GG( d, a, b, c, x[ 2], S22, 0xfcefa3f8 );
    GG( c, d, a, b, x[ 7], S23, 0x676f02d9 );
    GG( b, c, d, a, x[12], S24, 0x8d2a4c8a );
    HH( a, b, c, d, x[ 5], S31, 0xfffa3942 );
    HH( d, a, b, c, x[ 8], S32, 0x8771f681 );
    HH( c, d, a, b, x[11], S33, 0x6d9d6122 );
    HH( b, c, d, a, x[14], S34, 0xfde5380c );
    HH( a, b, c, d, x[ 1], S31, 0xa4beea44 );
    HH( d, a, b, c, x[ 4], S32, 0x4bdecfa9 );
    HH( c, d, a, b, x[ 7], S33, 0xf6bb4b60 );
    HH( b, c, d, a, x[10], S34, 0xbebfbc70 );
    HH( a, b, c, d, x[13], S31, 0x289b7ec6 );
    HH( d, a, b, c, x[ 0], S32, 0xeaa127fa );
    HH( c, d, a, b, x[ 3], S33, 0xd4ef3085 );
    HH( b, c, d, a, x[ 6], S34,  0x4881d05 );
    HH( a, b, c, d, x[ 9], S31, 0xd9d4d039 );
    HH( d, a, b, c, x[12], S32, 0xe6db99e5 );
    HH( c, d, a, b, x[15], S33, 0x1fa27cf8 );
    HH( b, c, d, a, x[ 2], S34, 0xc4ac5665 );
    II( a, b, c, d, x[ 0], S41, 0xf4292244 );
    II( d, a, b, c, x[ 7], S42, 0x432aff97 );
    II( c, d, a, b, x[14], S43, 0xab9423a7 );
    II( b, c, d, a, x[ 5], S44, 0xfc93a039 );
    II( a, b, c, d, x[12], S41, 0x655b59c3 );
    II( d, a, b, c, x[ 3], S42, 0x8f0ccc92 );
    II( c, d, a, b, x[10], S43, 0xffeff47d );
    II( b, c, d, a, x[ 1], S44, 0x85845dd1 );
    II( a, b, c, d, x[ 8], S41, 0x6fa87e4f );
    II( d, a, b, c, x[15], S42, 0xfe2ce6e0 );
    II( c, d, a, b, x[ 6], S43, 0xa3014314 );
    II( b, c, d, a, x[13], S44, 0x4e0811a1 );
    II( a, b, c, d, x[ 4], S41, 0xf7537e82 );
    II( d, a, b, c, x[11], S42, 0xbd3af235 );
    II( c, d, a, b, x[ 2], S43, 0x2ad7d2bb );
    II( b, c, d, a, x[ 9], S44, 0xeb86d391 );
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    memset( x, 0, sizeof x );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API


void
FMD5::Update( const unsigned char input[], uint32 length )
{
    uint32 index = count[0] / 8 % 64;

    if( ( count[0] += ( length << 3 ) ) < ( length << 3 ) )
        count[1]++;
    count[1] += ( length >> 29 );

    uint32 firstpart = 64 - index;
    uint32 i;

    if( length >= firstpart )
    {
        memcpy( &buffer[index], input, firstpart );
        Transform( buffer );

        for( i = firstpart; i + 64 <= length; i += 64 )
            Transform(&input [i] );

    index = 0;
    }
    else
    {
        i = 0;
    }

    memcpy( &buffer[index], &input[i], length-i );
}


void
FMD5::Update( const char input[], uint32 length )
{
    Update( (const unsigned char*)input, length );
}


FMD5&
FMD5::Finalize()
{
    static unsigned char padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if( !finalized ) {
        unsigned char bits[8];
        Encode( bits, count, 8 );

        uint32 index = count[0] / 8 % 64;
        uint32 padLen = ( index < 56 ) ? ( 56 - index ) : ( 120 - index );
        Update( padding, padLen );
        Update( bits, 8 );

        Encode(digest, state, 16);
        memset(buffer, 0, sizeof buffer);
        memset(count, 0, sizeof count);
        finalized=true;
    }

    return  *this;
}


std::string
FMD5::Hexdigest() const
{
    if( !finalized )
        return  "";

    char buf[33];
    for( int i=0; i < 16; i++ )
        sprintf( buf+i*2, "%02x", digest[i] );
    buf[32]=0;

    return  std::string( buf );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ Private logic


//static
void
FMD5::Decode( uint32 output[], const unsigned char input[], uint32 len )
{
    for( unsigned int i = 0, j = 0; j < len; i++, j += 4 )
        output[i] = ((uint32)input[j]) | (((uint32)input[j+1]) << 8) |
                    (((uint32)input[j+2]) << 16) | (((uint32)input[j+3]) << 24);
}


//static
void
FMD5::Encode( unsigned char output[], const uint32 input[], uint32 len )
{
    for( uint32 i = 0, j = 0; j < len; i++, j += 4 )
    {
        output[j] = input[i] & 0xff;
        output[j+1] = (input[i] >> 8) & 0xff;
        output[j+2] = (input[i] >> 16) & 0xff;
        output[j+3] = (input[i] >> 24) & 0xff;
    }
}


//static
inline
uint32
FMD5::F( uint32 x, uint32 y, uint32 z )
{
    return  (x&y) | (~x&z);
}


//static
inline
uint32
FMD5::G( uint32 x, uint32 y, uint32 z )
{
    return  (x&z) | (y&~z);
}


//static
inline
uint32
FMD5::H( uint32 x, uint32 y, uint32 z )
{
    return  x^y^z;
}


//static
inline
uint32
FMD5::I( uint32 x, uint32 y, uint32 z )
{
    return  y ^ (x | ~z);
}


//static
inline
uint32
FMD5::Rotate( uint32 x, int n )
{
    return  ( x << n ) | ( x >> ( 32-n ) );
}


//static
inline
void
FMD5::FF( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac )
{
    a = Rotate( a+ F( b, c, d ) + x + ac, s ) + b;
}


//static
inline
void
FMD5::GG( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac )
{
    a = Rotate( a + G( b, c, d ) + x + ac, s ) + b;
}


//static
inline
void
FMD5::HH( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac )
{
    a = Rotate( a + H( b, c, d ) + x + ac, s ) + b;
}


//static
inline
void
FMD5::II( uint32 &a, uint32 b, uint32 c, uint32 d, uint32 x, uint32 s, uint32 ac )
{
    a = Rotate( a + I( b, c, d ) + x + ac, s ) + b;
}


/////////////////////////////////////////////////////
// Accessibility util


std::string MD5(const std::string str)
{
    FMD5 m = FMD5(str);
    return  m.Hexdigest();
}


std::string MD5( const uint8* data, int length )
{
    FMD5 m = FMD5(data, length);
    return  m.Hexdigest();
}

} // namespace ULIS
