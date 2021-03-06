// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
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
    FHardwareMetrics host = FHardwareMetrics::Detect();
    uint32 perfIntent = 0 | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/round.png";
    FBlock* source = XLoadFromFile( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, path, ULIS_FORMAT_RGBA8 );
    FBlock* canvas = new  FBlock( 1200, 1000, ULIS_FORMAT_RGBA8 );
    FillPreserveAlpha( threadPool, ULIS_BLOCKING, 0, host, ULIS_NOCB, source, FColor( ULIS_FORMAT_RGB8, { 127, 127, 0 } ), source->Rect() );
    Fill( threadPool, ULIS_BLOCKING, 0, host, ULIS_NOCB, canvas, FColor( ULIS_FORMAT_RGB8, { 255, 255, 255 } ), canvas->Rect() );
    FTransformation2D transform( FTransformation2D::ComposeTransforms( FTransformation2D::MakeTranslationTransform( 20, 20 ), FTransformation2D::MakeRotationTransform( 3.14 / 4 ) ) );
    FBlock* transformed = XTransformAffine( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, source, source->Rect(), transform, INTERP_BILINEAR );

    FVec2F base( 200, 200 );
    FVec2F delta( 0.f, 0.f );
    for( int i = 0; i < 20; ++i ) {
        AlphaBlend( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, transformed, canvas, transformed->Rect(), base + delta * i, ULIS_NOAA, 1.f );
    }

    //TransformAffineTiled( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, source, canvas, source->Rect(), FRectI( 0, 0, 200, 200 ),FTransformation2D::MakeRotationTransform( FMath::kPIf / 4.f ), INTERP_BICUBIC );

    XDeleteThreadPool( threadPool );
    delete  source;
    delete  transformed;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas->Bits()
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
