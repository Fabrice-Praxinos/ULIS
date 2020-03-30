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
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | ULIS3_PERF_TSPEC | ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/over_160.png";
    FBlock* blockSRC = XLoadFromFile( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, path, ULIS3_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 1200, 1000, ULIS3_FORMAT_RGBA8 );
    Clear( &threadPool, ULIS3_BLOCKING, 0, host, ULIS3_NOCB, blockDST, blockDST->Rect() );
    Blend( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockSRC, blockDST, blockSRC->Rect(), FVec2F( 20, 20 ), ULIS3_NOAA, BM_NORMAL, AM_NORMAL, 1.f );

    delete  blockSRC;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->DataPtr()
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

    return  exit_code;
}
