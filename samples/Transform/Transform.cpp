// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul2;

int
main( int argc, char *argv[] ) {
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = 0 | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/base_160.png";
    FBlock* blockSRC = XLoadFromFile( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, path, ULIS2_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 800, 600, ULIS2_FORMAT_RGBA8 );

    Clear( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, blockDST, blockDST->Rect() );
    TransformAffine( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB
        , blockSRC, blockDST, FRect( 0, 0, 32, 32 )
        , FTransform2D( ComposeMatrix( ComposeMatrix( MakeScaleMatrix( 10, 10 ), MakeRotationMatrix( 3.14 / 4 ) ), MakeTranslationMatrix( 200, 50 ) ) )
        , INTERP_BICUBIC );

    std::vector< FVec2F > srcpoints = { FVec2F( 0, 0 ), FVec2F( 160, 0 ), FVec2F( 160, 160 ), FVec2F( 0, 160 ) };
    std::vector< FVec2F > dstpoints = { FVec2F( 10, 10 ), FVec2F( 130, 40 ), FVec2F( 120, 110 ), FVec2F( 20, 140 ) };
    FTransform2D persp( GetPerspectiveMatrix( srcpoints.data(), dstpoints.data() ) );
    TransformPerspective( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB
        , blockSRC, blockDST, blockSRC->Rect()
        , persp
        , INTERP_BICUBIC );

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

