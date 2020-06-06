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

    std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
    FBlock* blockSRC = XLoadFromFile( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, path, ULIS3_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 800, 600, ULIS3_FORMAT_RGBA8 );
    FRect srcRect = blockSRC->Rect();
    FVec2F srcSize( srcRect.w, srcRect.h );
    glm::mat3 shift = MakeTranslationMatrix( -srcRect.w / 2, -srcRect.h / 2 );
    Clear( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockDST, blockDST->Rect() );

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMaths::kPIf * 2;
        glm::mat3 rotation      = MakeRotationMatrix( angle );
        glm::mat3 translation   = MakeTranslationMatrix( i * 100 + 50, 50 );
        FTransform2D transform( ComposeMatrix( translation, ComposeMatrix( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_NN );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMaths::kPIf * 2;
        glm::mat3 rotation      = MakeRotationMatrix( angle );
        glm::mat3 translation   = MakeTranslationMatrix( i * 100 + 50, 150 );
        FTransform2D transform( ComposeMatrix( translation, ComposeMatrix( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_BILINEAR );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle = i / 5.f * FMaths::kPIf * 2;
        glm::mat3 rotation      = MakeRotationMatrix( angle );
        glm::mat3 translation   = MakeTranslationMatrix( i * 100 + 50, 250 );
        FTransform2D transform( ComposeMatrix( translation, ComposeMatrix( rotation, shift ) ) );
        TransformAffine( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_BICUBIC );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMaths::kPIf;
        float iangle =  angle + FMaths::kPIf;
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
        FTransform2D persp( GetPerspectiveMatrix( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_NN );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMaths::kPIf;
        float iangle =  angle + FMaths::kPIf;
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
        FTransform2D persp( GetPerspectiveMatrix( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_BILINEAR );
    }

    for( int i = 0; i < 5; ++i ) {
        float angle  = i / 5.f * FMaths::kPIf;
        float iangle =  angle + FMaths::kPIf;
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
        FTransform2D persp( GetPerspectiveMatrix( srcpoints.data(), dstpoints.data() ) );
        TransformPerspective( threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB
            , blockSRC, blockDST, blockSRC->Rect()
            , persp
            , INTERP_BICUBIC );
    }

    XDeleteThreadPool( threadPool );
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
