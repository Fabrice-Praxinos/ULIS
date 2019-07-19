/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Data.Block.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#pragma once

#include "ULIS/Base/ULIS.Base.AlignedMemory.h"
#include "ULIS/Base/ULIS.Base.CRC32.h"
#include "ULIS/Base/ULIS.Base.MD5.h"
#include "ULIS/Base/ULIS.Base.VectorSIMD.h"
#include "ULIS/Base/ULIS.Base.WeakUUID.h"
#include "ULIS/Data/ULIS.Data.Meta.h"
#include "ULIS/Data/ULIS.Data.Pixel.h"
#include "ULIS/Maths/ULIS.Maths.Geometry.h"

namespace ULIS {
/////////////////////////////////////////////////////
// Defines
#define tSpec TBlockInfo< _SH >

/////////////////////////////////////////////////////
// TBlockData
template< uint32 _SH >
class TBlockData final
{
public:
    // Construction / Destruction
    TBlockData()
        : width     ( 0         )
        , height    ( 0         )
        , data      ( nullptr   )
        , owned     ( true      )
    {}

    TBlockData( int iWidth, int iHeight )
        : width     ( iWidth    )
        , height    ( iHeight   )
        , data      ( nullptr   )
        , owned     ( true      )
    {
        data = new uint8[ BytesTotal() ];
    }

    ~TBlockData()
    {
        if( owned && data ) delete [] data;
    }

public:
    // Public API
    inline         uint8*                       DataPtr             ()                                                                          { return  data;                                                   }
    inline         const uint8*                 DataPtr             ()                                              const                       { return  data;                                                   }
    inline         uint8*                       PixelPtr            ( int x, int y )                                                            { return  DataPtr() + ( x * BytesPerPixel() + y * BytesPerScanLine() ); }
    inline         const uint8*                 PixelPtr            ( int x, int y )                                const                       { return  DataPtr() + ( x * BytesPerPixel() + y * BytesPerScanLine() ); }
    inline         uint8*                       ScanlinePtr         ( int row )                                                                 { return  DataPtr() + ( row * BytesPerScanLine() );                     }
    inline         const uint8*                 ScanlinePtr         ( int row )                                     const                       { return  DataPtr() + ( row * BytesPerScanLine() );                     }
    inline         int                          Depth               ()                                              const                       { return  tSpec::_nf._pd;                                               }
    inline         int                          Width               ()                                              const                       { return  width;                                                        }
    inline         int                          Height              ()                                              const                       { return  height;                                                       }
    inline         int                          BytesPerPixel       ()                                              const                       { return  Depth();                                                      }
    inline         int                          BytesPerScanLine    ()                                              const                       { return  Depth() * Width();                                            }
    inline         int                          BytesTotal          ()                                              const                       { return  BytesPerScanLine() * Height();                                }
    inline         CColor                       PixelColor          ( int x, int y )                                                            { return  TPixelProxy< _SH >( (uint8*)PixelPtr( x, y ) ).GetColor();    }
    inline         CColor                       PixelColor          ( int x, int y )                                const                       { return  TPixelProxy< _SH >( (uint8*)PixelPtr( x, y ) ).GetColor();    }
    inline         TPixelProxy< _SH >           PixelProxy          ( int x, int y )                                                            { return  TPixelProxy< _SH >( (uint8*)PixelPtr( x, y ) );               }
    inline         TPixelProxy< _SH >           PixelProxy          ( int x, int y )                                const                       { return  TPixelProxy< _SH >( (uint8*)PixelPtr( x, y ) );               }
    inline         TPixelValue< _SH >           PixelValue          ( int x, int y )                                                            { return  TPixelValue< _SH >( PixelProxy( x, y ) );                     }
    inline         TPixelValue< _SH >           PixelValue          ( int x, int y )                                const                       { return  TPixelValue< _SH >( PixelProxy( x, y ) );                     }
    inline         void                         SetPixelColor       ( int x, int y, const CColor& iValue )                                      { PixelProxy( x, y ).SetColor( iValue );                                }
    inline         void                         SetPixelValue       ( int x, int y, const TPixelValue< _SH >& iValue )                          { PixelProxy( x, y ) = iValue;                                          }
    inline         void                         SetPixelProxy       ( int x, int y, const TPixelProxy< _SH >& iValue )                          { PixelProxy( x, y ) = iValue;                                          }

private:
    // Private Data
    uint32  width;
    uint32  height;
    uint8*  data;
    bool owned;
};


/////////////////////////////////////////////////////
// IBlock
class IBlock
{
public:
    // Typedef
    typedef void (*fpInvalidateFunction)( IBlock* /*data*/, void* /*info*/, const FRect& /*rect*/ );

public:
    // Construction / Destruction
    IBlock()
        : mInvCb(   nullptr )
        , mInvInfo( nullptr )
    {}
    virtual ~IBlock() {} // Polymorphic

public:
    // Public API
           virtual const char*                  Name                ()                                              const                       = 0;
           virtual const uint32                 Id                  ()                                              const                       = 0;
           virtual uint8*                       DataPtr             ()                                                                          = 0;
           virtual const uint8*                 DataPtr             ()                                              const                       = 0;
           virtual uint8*                       PixelPtr            ( int x, int y )                                                            = 0;
           virtual const uint8*                 PixelPtr            ( int x, int y )                                const                       = 0;
           virtual uint8*                       ScanlinePtr         ( int row )                                                                 = 0;
           virtual const uint8*                 ScanlinePtr         ( int row )                                     const                       = 0;
           virtual int                          Depth               ()                                              const                       = 0;
           virtual int                          Width               ()                                              const                       = 0;
           virtual int                          Height              ()                                              const                       = 0;
           virtual double                       MaxD                ()                                              const                       = 0;
           virtual int64                        MaxI                ()                                              const                       = 0;
           virtual double                       RangeD              ()                                              const                       = 0;
           virtual int64                        RangeI              ()                                              const                       = 0;
           virtual int                          BytesPerPixel       ()                                              const                       = 0;
           virtual int                          BytesPerScanLine    ()                                              const                       = 0;
           virtual int                          BytesTotal          ()                                              const                       = 0;
           virtual e_tp                         Type                ()                                              const                       = 0;
           virtual e_cm                         ColorModel          ()                                              const                       = 0;
           virtual e_ea                         ExtraAlpha          ()                                              const                       = 0;
           virtual bool                         HasAlpha            ()                                              const                       = 0;
           virtual const char*                  ChannelLayout       ()                                              const                       = 0;
           virtual e_nm                         NormalMode          ()                                              const                       = 0;
           virtual bool                         IsNormalized        ()                                              const                       = 0;
           virtual bool                         IsDecimal           ()                                              const                       = 0;
           virtual int                          NumChannels         ()                                              const                       = 0;
           virtual int                          NumColorChannels    ()                                              const                       = 0;
           virtual std::string                  MD5Hash             ()                                              const                       = 0;
           virtual uint32                       CRC32Hash           ()                                              const                       = 0;
           virtual std::string                  UUID                ()                                              const                       = 0;
           virtual CColor                       PixelColor          ( int x, int y )                                                            = 0;
           virtual CColor                       PixelColor          ( int x, int y )                                const                       = 0;
           virtual void                         SetPixelColor       ( int x, int y, const CColor& iColor )                                      = 0;
           virtual void                         Fill                ( const CColor& iColor )                                                    = 0;
           virtual void                         Clear               ()                                                                          = 0;
           virtual void                         Clear               ( const FRect& iRect )                                                      = 0;
                   void                         Invalidate          ()                                                                          { if( mInvCb ) mInvCb( this, mInvInfo, { 0, 0, Width(), Height() } );   }
                   void                         Invalidate          ( const FRect& iRect )                                                      { if( mInvCb ) mInvCb( this, mInvInfo, iRect );                         }
                   void                         SetInvalidateCB     ( fpInvalidateFunction iCb, void* iInfo )                                   { mInvCb = iCb; mInvInfo = iInfo;                                       }

protected:
    // Protected Data
    fpInvalidateFunction    mInvCb;
    void*                   mInvInfo;
    FBlockMetaData          meta;
};


/////////////////////////////////////////////////////
// TBlock
template< uint32 _SH >
class TBlock final : public IBlock
{
public:
    // Typedef
    typedef TPixelValue< _SH > tPixelValue;
    typedef TPixelProxy< _SH > tPixelProxy;
    using tPixelType        = typename TPixelTypeSelector< tSpec::_nf._tp >::_tUnderlyingPixelType;
    using tNextPixelType    = typename TPixelTypeSelector< tSpec::_nf._tp >::_tUnderlyingNextPixelType;
    using tPrevPixelType    = typename TPixelTypeSelector< tSpec::_nf._tp >::_tUnderlyingPrevPixelType;

public:
    // Construction / Destruction
    virtual ~TBlock() { delete d; } // Polymorphic

    TBlock( int iWidth, int iHeight )
        : d     ( new TBlockData< _SH >( iWidth, iHeight )  )
        , id    ( generate_weak_uuid( 16 )                  )
        , IBlock()
    {}

public:
    static tPixelType   StaticFastMax() { return (tPixelType)std::numeric_limits< tPixelType >::max(); }
    static tPixelType   StaticMax() { return (tPixelType)tSpec::_nf._tm; }
    static int          StaticNumChannels         () { return  tSpec::_nf._rc; }
    static int          StaticNumColorChannels    () { return  tSpec::_nf._nc; }

public:
    // Template API
    template< typename T > inline T             MaxT                ()                                              const                       { return  (T)tSpec::_nf._tm;                                            }
    template< typename T > inline T             RangeT              ()                                              const                       { return  (T)tSpec::_nf._rm;                                            }
    inline         tPixelType                   Max                 ()                                              const                       { return  MaxT< tPixelType >();                                         }
    inline         tNextPixelType               Range               ()                                              const                       { return  RangeT< tNextPixelType >();                                   }

public:
    // Public API
    inline virtual const char*                  Name                ()                                              const   override    final   { return  tSpec::_nf._ss;                                               }
    inline virtual const uint32                 Id                  ()                                              const   override    final   { return  tSpec::_nf._sh;                                               }
    inline virtual uint8*                       DataPtr             ()                                                      override    final   { return  d->DataPtr();                                                 }
    inline virtual const uint8*                 DataPtr             ()                                              const   override    final   { return  d->DataPtr();                                                 }
    inline virtual uint8*                       PixelPtr            ( int x, int y )                                        override    final   { return  d->PixelPtr( x, y );                                          }
    inline virtual const uint8*                 PixelPtr            ( int x, int y )                                const   override    final   { return  d->PixelPtr( x, y );                                          }
    inline virtual uint8*                       ScanlinePtr         ( int row )                                             override    final   { return  d->ScanlinePtr( row );                                        }
    inline virtual const uint8*                 ScanlinePtr         ( int row )                                     const   override    final   { return  d->ScanlinePtr( row );                                        }
    inline virtual int                          Depth               ()                                              const   override    final   { return  d->Depth();                                                   }
    inline virtual int                          Width               ()                                              const   override    final   { return  d->Width();                                                   }
    inline virtual int                          Height              ()                                              const   override    final   { return  d->Height();                                                  }
    inline virtual double                       MaxD                ()                                              const   override    final   { return  MaxT< double >();                                             }
    inline virtual int64                        MaxI                ()                                              const   override    final   { return  MaxT< int64 >();                                              }
    inline virtual double                       RangeD              ()                                              const   override    final   { return  RangeT< double >();                                           }
    inline virtual int64                        RangeI              ()                                              const   override    final   { return  RangeT< int64 >();                                            }
    inline virtual int                          BytesPerPixel       ()                                              const   override    final   { return  d->BytesPerPixel();                                           }
    inline virtual int                          BytesPerScanLine    ()                                              const   override    final   { return  d->BytesPerScanLine();                                        }
    inline virtual int                          BytesTotal          ()                                              const   override    final   { return  d->BytesTotal();                                              }
    inline virtual e_tp                         Type                ()                                              const   override    final   { return  tSpec::_nf._tp;                                               }
    inline virtual e_cm                         ColorModel          ()                                              const   override    final   { return  tSpec::_nf._cm;                                               }
    inline virtual e_ea                         ExtraAlpha          ()                                              const   override    final   { return  tSpec::_nf._ea;                                               }
    inline virtual bool                         HasAlpha            ()                                              const   override    final   { return  ExtraAlpha() == e_ea::khasAlpha;                              }
    inline virtual const char*                  ChannelLayout       ()                                              const   override    final   { return  tSpec::_nf._cl;                                               }
    inline virtual e_nm                         NormalMode          ()                                              const   override    final   { return  tSpec::_nf._nm;                                               }
    inline virtual bool                         IsNormalized        ()                                              const   override    final   { return  NormalMode() == e_nm::knormalized;                            }
    inline virtual bool                         IsDecimal           ()                                              const   override    final   { return  tSpec::_nf._dm;                                               }
    inline virtual int                          NumChannels         ()                                              const   override    final   { return  tSpec::_nf._rc;                                               }
    inline virtual int                          NumColorChannels    ()                                              const   override    final   { return  tSpec::_nf._nc;                                               }
    inline virtual std::string                  MD5Hash             ()                                              const   override    final   { return  MD5( DataPtr(), BytesTotal() );                               }
    inline virtual uint32                       CRC32Hash           ()                                              const   override    final   { return  CRC32( DataPtr(), BytesTotal() );                             }
    inline virtual std::string                  UUID                ()                                              const   override    final   { return  id;                                                           }
    inline virtual CColor                       PixelColor          ( int x, int y )                                        override    final   { return  d->PixelColor( x, y );                                        }
    inline virtual CColor                       PixelColor          ( int x, int y )                                const   override    final   { return  d->PixelColor( x, y );                                        }
    inline virtual void                         SetPixelColor       ( int x, int y, const CColor& iValue )                  override    final   { d->SetPixelColor( x, y, iValue );                                     }
    inline         tPixelValue                  PixelValue          ( int x, int y )                                                            { return  d->PixelValue( x, y );                                        }
    inline         tPixelValue                  PixelValue          ( int x, int y )                                const                       { return  d->PixelValue( x, y );                                        }
    inline         tPixelProxy                  PixelProxy          ( int x, int y )                                                            { return  d->PixelProxy( x, y );                                        }
    inline         tPixelProxy                  PixelProxy          ( int x, int y )                                const                       { return  d->PixelProxy( x, y );                                        }
    inline         void                         SetPixelValue       ( int x, int y, const tPixelValue& iValue )                                 { d->SetPixelValue( x, y, iValue );                                     }
    inline         void                         SetPixelProxy       ( int x, int y, const tPixelProxy& iValue )                                 { d->SetPixelProxy( x, y, iValue );                                     }
    inline virtual void                         Clear               ()                                                      override    final   { memset( DataPtr(), 0, BytesTotal() ); }

    virtual void Fill( const CColor& iColor ) override final {
        tPixelValue val;
        val.SetColor( iColor.ToModel( CColorModelFromColorModel( ColorModel() ) ) );
        const int w = Width();
        const int h = Height();
        for( int y = 0; y < h; ++y )
            for( int x = 0; x < w; ++x )
                SetPixelValue( x, y, val );
    }

    virtual void Clear( const FRect& iRect ) override final {
            int xmin = FMath::Max( iRect.x, 0 );
            int ymin = FMath::Max( iRect.y, 0 );
            int xmax = FMath::Min( iRect.x + iRect.w, Width() );
            int ymax = FMath::Min( iRect.y + iRect.h, Height() );

            int bytesToClear = ( xmax - xmin ) * BytesPerPixel();
            int shift = xmin * BytesPerPixel();

            for( int y = ymin; y < ymax; ++y )
                memset( ScanlinePtr( y ) + shift, 0, bytesToClear );
    }

public:
    // Constexpr API
    inline static  constexpr FSpec        TypeSpec            ()                                                                          { return  tSpec::_nf;                                                   }
    inline static  constexpr char*        TypeStr             ()                                                                          { return  tSpec::_nf._ss;                                               }
    inline static  constexpr uint32       TypeId              ()                                                                          { return  tSpec::_nf._sh;                                               }

private:
    // Private Data
    TBlockData< _SH >*  d;
    std::string         id;
};


/////////////////////////////////////////////////////
// Undefines
#undef tSpec


} // namespace ULIS

