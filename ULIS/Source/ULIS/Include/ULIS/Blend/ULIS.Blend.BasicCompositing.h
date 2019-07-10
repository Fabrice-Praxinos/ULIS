/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Blend.BasicCompositing.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#pragma once

#include <assert.h>
#include "ULIS/Base/ULIS.Base.BaseTypes.h"
#include "ULIS/Blend/ULIS.Blend.BlendingModes.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Blend/ULIS.Blend.BlendFunc.h"

namespace ULIS {
/////////////////////////////////////////////////////
// Defines
#define tSpec TBlockInfo< _SH >

/////////////////////////////////////////////////////
// Union Compositing
template< uint32 _SH >
static inline
typename TBlock< _SH >::tPixelType
Union( typename TBlock< _SH >::tPixelType b,
       typename TBlock< _SH >::tPixelType s )
{
    return  ( b + s ) - ConvType< typename TBlock< _SH >::tNextPixelType, typename TBlock< _SH >::tPixelType >( (typename TBlock< _SH >::tNextPixelType)( b * s ) );
}

/////////////////////////////////////////////////////
// Composer / BasicCompositing
template< uint32 _SH, eBlendingMode _BM >
struct Composer
{
    static
    typename TBlock< _SH >::tPixelType
    BasicCompositing( typename TBlock< _SH >::tPixelType Cb, typename TBlock< _SH >::tPixelType Cs, typename TBlock< _SH >::tPixelType ab, typename TBlock< _SH >::tPixelType var ) {
        return  ConvType< typename TBlock< _SH >::tNextPixelType, typename TBlock< _SH >::tPixelType >( ( TBlock< _SH >::StaticMax() - var ) * Cb   +   var * ConvType< typename TBlock< _SH >::tNextPixelType, typename TBlock< _SH >::tPixelType >( ( TBlock< _SH >::StaticMax() - ab ) * Cs + ab * BlendFunc< _SH, _BM >::Compute( Cb, Cs ) ) );
    }
};

/////////////////////////////////////////////////////
// Undefines
#undef tSpec

} // namespace ULIS