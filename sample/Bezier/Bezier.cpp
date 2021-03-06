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

    std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
    FBlock* blockSRC = XLoadFromFile( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, path, ULIS_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 1024, 1024, ULIS_FORMAT_RGBA8 );
    Clear( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockDST, blockDST->Rect() );

    int size = 256;
    int shift = 100;
    FVec2F A( shift, shift );
    FVec2F B( shift + size, shift );
    FVec2F C( shift + size, shift + size );
    FVec2F D( shift, shift + size );
    std::vector< FCubicBezierControlPoint > ctrlpoints = { { A, A + FVec2F( 50, -50 ), A }
                                                         , { B, B, B }
                                                         , { C, C + FVec2F( -50, -50 ), C }
                                                         , { D, D + FVec2F( 50, -50 ), D } };
    TransformBezier( threadPool, ULIS_NONBLOCKING, perfIntent, host, ULIS_NOCB, blockSRC, blockDST, blockSRC->Rect(), ctrlpoints, 0.5f, 1, INTERP_NN );

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->Bits()
                                        , blockDST->Width()
                                        , blockDST->Height()
                                        , blockDST->BytesPerScanLine()
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

    delete  blockDST;
    delete  blockSRC;

    XDeleteThreadPool( threadPool );
    return  exit_code;
}

