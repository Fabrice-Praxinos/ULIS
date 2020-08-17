// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Device_Linux.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FDevice tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/Device.h"
#include "System/DeviceHelpers.h"

#include <cpuid.h>
#include <intrin.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <thread>
#include <stdio.h>

ULIS_NAMESPACE_BEGIN

namespace detail {

#define _XCR_XFEATURE_ENABLED_MASK  0
void
cpuid( int32_t out[4], int32_t x ) {
    __cpuid_count( x, 0, out[0], out[1], out[2], out[3] );
}

uint64_t
xgetbv( unsigned int index ) {
    uint32_t eax, edx;
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
    return ( ( uint64_t )edx << 32 ) | eax;
}

bool
detect_OS_x64(){
    //  We only support x64
    return true;
}

bool
detect_OS_AVX() {
    //  Copied from: http://stackoverflow.com/a/22521619/922184
    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo, 1);

    bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
    bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        uint64_t xcrFeatureMask = xgetbv( _XCR_XFEATURE_ENABLED_MASK );
        avxSupported = ( xcrFeatureMask & 0x6 ) == 0x6;
    }

    return  avxSupported;
}

bool
detect_OS_AVX512() {
    if (!detect_OS_AVX())
        return  false;

    uint64_t xcrFeatureMask = xgetbv( _XCR_XFEATURE_ENABLED_MASK );
    return  ( xcrFeatureMask & 0xe6 ) == 0xe6;
}

std::string
get_vendor_string() {
    int32_t CPUInfo[4];
    char name[13];

    cpuid(CPUInfo, 0);
    memcpy(name + 0, &CPUInfo[1], 4);
    memcpy(name + 4, &CPUInfo[3], 4);
    memcpy(name + 8, &CPUInfo[2], 4);
    name[12] = '\0';

    return  name;
}

size_t cache_line_size() {
    FILE * p = 0;
    p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
    unsigned int i = 0;
    if (p) {
        fscanf(p, "%d", &i);
        fclose(p);
    }
    return i;
}


size_t cache_size() {
    FILE * p = 0;
    p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/size", "r");
    unsigned int i = 0;
    if (p) {
        fscanf(p, "%d", &i);
        fclose(p);
    }
    return i;
}

} // namespace detail

FDevice::FDevice()
    : bField( 0 )
    , mMaxWorkers( std::thread::hardware_concurrency() )
    , mL1CacheSize( detail::cache_size() )
    , mL1CacheLineSize( detail::cache_line_size() )
{
    bField |= ULIS_W_OS_X64( detail::detect_OS_x64() );
    bField |= ULIS_W_OS_AVX( detail::detect_OS_AVX() );
    bField |= ULIS_W_OS_AVX512( detail::detect_OS_AVX512() );

    std::string vendor( detail::get_vendor_string() );
    if( vendor == "GenuineIntel" )
        bField |= ULIS_R_HW_INTEL( 1 );
    else if( vendor == "AuthenticAMD")
        bField |= ULIS_R_HW_AMD( 1 );

    int info[4] { 0, 0, 0, 0 };
    detail::cpuid( info, 0 );
    int nIds = info[0];

    detail::cpuid( info, 0x80000000 );
    uint32_t nExIds = info[0];

    if( nIds >= 0x00000001 ) {
        detail::cpuid( info, 0x00000001 );
        bField |= ULIS_W_HW_MMX(    info[3] & ( 1 << 23 ) );
        bField |= ULIS_W_HW_SSE(    info[3] & ( 1 << 25 ) );
        bField |= ULIS_W_HW_SSE2(   info[3] & ( 1 << 26 ) );
        bField |= ULIS_W_HW_SSE3(   info[2] & ( 1 << 0  ) );
        bField |= ULIS_W_HW_SSSE3(  info[2] & ( 1 << 9  ) );
        bField |= ULIS_W_HW_SSE41(  info[2] & ( 1 << 19 ) );
        bField |= ULIS_W_HW_SSE42(  info[2] & ( 1 << 20 ) );
        bField |= ULIS_W_HW_AES(    info[2] & ( 1 << 25 ) );
        bField |= ULIS_W_HW_AVX(    info[2] & ( 1 << 28 ) );
        bField |= ULIS_W_HW_FMA3(   info[2] & ( 1 << 12 ) );
        bField |= ULIS_W_HW_RDRAND( info[2] & ( 1 << 30 ) );
    }

    if( nIds >= 0x00000007 ) {
        detail::cpuid( info, 0x00000007 );
        bField |= ULIS_W_HW_AVX2(           info[1] & ( 1 << 5  ) );
        bField |= ULIS_W_HW_BMI1(           info[1] & ( 1 << 3  ) );
        bField |= ULIS_W_HW_BMI2(           info[1] & ( 1 << 8  ) );
        bField |= ULIS_W_HW_ADX(            info[1] & ( 1 << 19 ) );
        bField |= ULIS_W_HW_MPX(            info[1] & ( 1 << 14 ) );
        bField |= ULIS_W_HW_SHA(            info[1] & ( 1 << 29 ) );
        bField |= ULIS_W_HW_PREFETCHWT1(    info[2] & ( 1 << 0  ) );
        bField |= ULIS_W_HW_AVX512_F(       info[1] & ( 1 << 16 ) );
        bField |= ULIS_W_HW_AVX512_CD(      info[1] & ( 1 << 28 ) );
        bField |= ULIS_W_HW_AVX512_PF(      info[1] & ( 1 << 26 ) );
        bField |= ULIS_W_HW_AVX512_ER(      info[1] & ( 1 << 27 ) );
        bField |= ULIS_W_HW_AVX512_VL(      info[1] & ( 1 << 31 ) );
        bField |= ULIS_W_HW_AVX512_BW(      info[1] & ( 1 << 30 ) );
        bField |= ULIS_W_HW_AVX512_DQ(      info[1] & ( 1 << 17 ) );
        bField |= ULIS_W_HW_AVX512_IFMA(    info[1] & ( 1 << 21 ) );
        bField |= ULIS_W_HW_AVX512_VBMI(    info[2] & ( 1 << 1  ) );
    }

    if( nExIds >= 0x80000001 ) {
        detail::cpuid( info, 0x80000001 );
        bField |= ULIS_W_HW_X64(    info[3] & ( 1 << 29 ) );
        bField |= ULIS_W_HW_ABM(    info[2] & ( 1 << 5  ) );
        bField |= ULIS_W_HW_SSE4A(  info[2] & ( 1 << 6  ) );
        bField |= ULIS_W_HW_FMA4(   info[2] & ( 1 << 16 ) );
        bField |= ULIS_W_HW_XOP(    info[2] & ( 1 << 11 ) );
    }
}

ULIS_NAMESPACE_END

