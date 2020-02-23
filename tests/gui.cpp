// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         gui.cpp
* @author       Clement Berthaud
* @brief        Gui application for ULIS2.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>
#include <chrono>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

int
main( int argc, char *argv[] ) {
    using namespace ::ul2;

    FThreadPool  threadPool;
    FPerf perfIntent_NONE( 0 );
    FPerf perfIntent_MT( Perf_MT );
    FPerf perfIntent_SSE_TSPEC( Perf_SSE4_2 | Perf_TSPEC );
    FPerf perfIntent_AVX_TSPEC( Perf_AVX2   | Perf_TSPEC );

    FPerfInfo perfInfo = {};
    perfInfo.pool       = &threadPool;
    perfInfo.intent     = perfIntent_AVX_TSPEC;
    perfInfo.blocking   = ULIS2_NONBLOCKING;
    perfInfo.callCB     = ULIS2_NOCB;

    FBlock       blockA( 800, 600,   ULIS2_FORMAT_RGBA8 );
    FPixelValue  red(    ULIS2_FORMAT_RGBA8, { 255, 0, 0, 255 } );
    FPixelValue  green(  ULIS2_FORMAT_RGBA8, { 0, 255, 0, 255 } );

    FFillInfo fillInfo = {};
    fillInfo.destination    = &blockA;
    fillInfo.color          = &green;
    fillInfo.area           = blockA.Rect();
    fillInfo.perfInfo       = perfInfo;
    Fill( fillInfo );

    FXLoadFromFileInfo loadInfo = {};
    loadInfo.path           = "C:/Users/conta/Documents/work/pattern.png";
    loadInfo.desiredFormat  = ULIS2_FORMAT_RGBA8;
    loadInfo.perfInfo       = perfInfo;
    FBlock* blockB = XLoadFromFile( loadInfo );

    Fence( threadPool );

    //SaveToFile( &threadPool, true, perfIntent_AVX_TSPEC, gCpuInfo, &blockB, "here.png", eImageFormat::IM_PNG );

    FBlendInfo blendInfo = {};
    blendInfo.source            = blockB;
    blendInfo.backdrop          = &blockA;
    blendInfo.sourceRect        = FRect( 0, 0, 65, 65 );
    blendInfo.backdropPosition  = FVec2F( 64.5f, 64 );
    blendInfo.subpixelFlag      = ULIS2_AA;
    blendInfo.blendingMode      = BM_NORMAL;
    blendInfo.alphaMode         = AM_NORMAL;
    blendInfo.opacityValue      = 1.f;
    blendInfo.perfInfo          = perfInfo;
    Blend( blendInfo );

    // Qt Window
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockA.DataPtr(), blockA.Width(), blockA.Height(), blockA.BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    delete  blockB;

    return  exit_code;
}

