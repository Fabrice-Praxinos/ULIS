/*************************************************************************
*
*   ULIS
*__________________
*
* MainWindow.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include <QWidget>

class QScrollArea;
namespace ULIS { class IBlock; }
class cTriVignette;

/////////////////////////////////////////////////////
// cMainWindow
class cMainWindow : public QWidget
{
    Q_OBJECT

    typedef QWidget     tSuperClass;
    typedef cMainWindow tSelf;

public:
    // Construction / Destruction
    cMainWindow( QWidget* iParent = nullptr );
    ~cMainWindow();

protected:
    // Protected Qt event overrides
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    // GUI utilities
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Data
    QScrollArea*    mScrollArea;
    QWidget*        mCanvas;
    ::ULIS::IBlock* mUnderSource;
    ::ULIS::IBlock* mOverSource;
    QVector< cTriVignette* > vignettes_list;
};

