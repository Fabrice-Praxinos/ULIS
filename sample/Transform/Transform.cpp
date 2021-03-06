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
    uint32 perfIntent = ULIS_PERF_SSE42;
    //uint32 perfIntent = 0;

    std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
    FBlock* blockSRC = XLoadFromFile( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, path, ULIS_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 800, 600, ULIS_FORMAT_RGBA8 );
    FRectI srcRect = blockSRC->Rect();
    FVec2F srcSize( srcRect.w, srcRect.h );
    FTransformation2D shift( FTransformation2D::MakeTranslationTransform( -srcRect.w / 2, -srcRect.h / 2 ) );
    Clear( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, blockDST, blockDST->Rect() );

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMath::kPIf * 2;
        FTransformation2D rotation( FTransformation2D::MakeRotationTransform( angle ) );
        FTransformation2D translation( FTransformation2D::MakeTranslationTransform( i * 100 + 50, 50 ) );
        FTransformation2D transform( FTransformation2D::ComposeTransforms( translation, FTransformation2D::ComposeTransforms( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_NN );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMath::kPIf * 2;
        FTransformation2D rotation( FTransformation2D::MakeRotationTransform( angle ) );
        FTransformation2D translation( FTransformation2D::MakeTranslationTransform( i * 100 + 50, 150 ) );
        FTransformation2D transform( FTransformation2D::ComposeTransforms( translation, FTransformation2D::ComposeTransforms( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_BILINEAR );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMath::kPIf * 2;
        FTransformation2D rotation( FTransformation2D::MakeRotationTransform( angle ) );
        FTransformation2D translation( FTransformation2D::MakeTranslationTransform( i * 100 + 50, 250 ) );
        FTransformation2D transform( FTransformation2D::ComposeTransforms( translation, FTransformation2D::ComposeTransforms( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_BICUBIC );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMath::kPIf;
        float iangle =  angle + FMath::kPIf;
        float rotx0 = ( cos( iangle ) + 1 ) / 2;
        float rotx1 = ( cos(  angle ) + 1 ) / 2;
        float dif = sin( angle ) * 0.15f;
        std::vector< FVec2F > srcpoints = { FVec2F( 0, 0 ), FVec2F( srcSize.x, 0 ), FVec2F( srcSize.x, srcSize.y ), FVec2F( 0, srcSize.y ) };
        std::vector< FVec2F > dstpoints = { FVec2F( rotx0, -dif ), FVec2F( rotx1, dif ), FVec2F( rotx1, 1 - dif ), FVec2F( rotx0, 1 + dif ) };
        FVec2F translation( i * 100 + 18, 350 - 32 );
        for( int i = 0; i < 4; ++i ) {
            dstpoints[i] *= srcSize;
            dstpoints[i] += translation;
        }
        FTransformation2D persp( FTransformation2D::GetPerspectiveTransform( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_NN );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMath::kPIf;
        float iangle =  angle + FMath::kPIf;
        float rotx0 = ( cos( iangle ) + 1 ) / 2;
        float rotx1 = ( cos(  angle ) + 1 ) / 2;
        float dif = sin( angle ) * 0.15f;
        std::vector< FVec2F > srcpoints = { FVec2F( 0, 0 ), FVec2F( srcSize.x, 0 ), FVec2F( srcSize.x, srcSize.y ), FVec2F( 0, srcSize.y ) };
        std::vector< FVec2F > dstpoints = { FVec2F( rotx0, -dif ), FVec2F( rotx1, dif ), FVec2F( rotx1, 1 - dif ), FVec2F( rotx0, 1 + dif ) };
        FVec2F translation( i * 100 + 18, 450 - 32 );
        for( int i = 0; i < 4; ++i ) {
            dstpoints[i] *= srcSize;
            dstpoints[i] += translation;
        }
        FTransformation2D persp( FTransformation2D::GetPerspectiveTransform( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_BILINEAR );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMath::kPIf;
        float iangle =  angle + FMath::kPIf;
        float rotx0 = ( cos( iangle ) + 1 ) / 2;
        float rotx1 = ( cos(  angle ) + 1 ) / 2;
        float dif = sin( angle ) * 0.15f;
        std::vector< FVec2F > srcpoints = { FVec2F( 0, 0 ), FVec2F( srcSize.x, 0 ), FVec2F( srcSize.x, srcSize.y ), FVec2F( 0, srcSize.y ) };
        std::vector< FVec2F > dstpoints = { FVec2F( rotx0, -dif ), FVec2F( rotx1, dif ), FVec2F( rotx1, 1 - dif ), FVec2F( rotx0, 1 + dif ) };
        FVec2F translation( i * 100 + 18, 550 - 32 );
        for( int i = 0; i < 4; ++i ) {
            dstpoints[i] *= srcSize;
            dstpoints[i] += translation;
        }
        FTransformation2D persp( FTransformation2D::GetPerspectiveTransform( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_BICUBIC );
    }

    XDeleteThreadPool( threadPool );
    delete  blockSRC;

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

    return  exit_code;
}
