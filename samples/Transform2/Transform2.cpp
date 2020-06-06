// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS3.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool* threadPool = XCreateThreadPool();
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = 0 | ULIS3_PERF_TSPEC | ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/round.png";
    FBlock* source = XLoadFromFile( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, path, ULIS3_FORMAT_RGBA8 );
    FBlock* canvas = new  FBlock( 1200, 1000, ULIS3_FORMAT_RGBA8 );
    Fill( threadPool, ULIS3_BLOCKING, 0, host, ULIS3_NOCB, canvas, FPixelValue( ULIS3_FORMAT_RGB8, { 20, 127, 200 } ), canvas->Rect() );
    FTransform2D transform = ComposeMatrix( MakeTranslationMatrix( 20, 20 ), MakeScaleMatrix( 4, 4 ) );
    FBlock* transformed = XResize( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, source, source->Rect(), FVec2F( 64, 64 ), INTERP_AREA );

    FVec2F base( 20, 20 );
    FVec2F delta( 0.5f, 0.5f );
    for( int i = 0; i < 200; ++i ) {
        Blend( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, transformed, canvas, transformed->Rect(), base + delta * i, ULIS3_AA, BM_NORMAL, AM_NORMAL, 1.f );
    }

    XDeleteThreadPool( threadPool );
    delete  source;
    delete  transformed;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas->DataPtr()
                                        , canvas->Width()
                                        , canvas->Height()
                                        , canvas->BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    delete  canvas;

    return  exit_code;
}
