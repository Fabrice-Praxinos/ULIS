/**
*
*   ULIS
*__________________
*
* @file     Application.Widgets.Slider.h
* @author   Clement Berthaud
* @brief    This file provides the declaration for the SSlider class.
*/
#pragma once

#include <QWidget>

class QTimer;

namespace  nApplication {
namespace  nWidgets {


class SSlider : public QWidget
{
    Q_OBJECT

    typedef  QWidget    tSuperClass;
    typedef  SSlider    tSelf;

public:
    // Construction / Destruction
    virtual  ~SSlider();
    SSlider( QWidget* iParent = nullptr );

protected:
    // Protected Qt event overrides
    virtual  void  paintEvent( QPaintEvent* event )  override;
    virtual  void  mousePressEvent( QMouseEvent*  event )  override;
    virtual  void  mouseMoveEvent( QMouseEvent*  event )  override;
    virtual  void  mouseReleaseEvent( QMouseEvent*  event )  override;

public slots:
    // Slots
    void  AutoAnimSlider();

signals:
    // Signals
    void  ValueChanged( float );

private:
    bool bIsLeftMouseDown;
    float mValue;
    QTimer* mTimer;
    bool  bSwapDir;
};


} // namespace  nWidgets
} // namespace  nApplication
