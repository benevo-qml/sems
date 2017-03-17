#include "ApplicationWindow.h"

#include <QTimer>
#include <QDebug>
#include <QQmlComponent>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#ifdef Q_OS_WIN
#include <winuser.h>
#elif defined(Q_OS_LINUX)
#include <X11/extensions/shape.h>
#include <QX11Info>
#endif
#include <QWidget>

bool test(QObject *, Qt::ShortcutContext)
{
    return true;
}

ApplicationWindow::ApplicationWindow(QQuickWindow *parent) :
    QQuickWindow(parent)
{
    setObjectName ("ApplicationWindow");
    
    m_noBorder = false;
    m_windowStatus = StopCenter;
    m_topHint = false;
    old_topHint=false;
    m_noNotifyIcon = false;
    m_windowActive = false;
    m_mousePenetrate = false;
    m_minimumWidth = 0;
    m_minimumHeight = 0;
    m_maximumWidth = 9999999;
    m_maximumHeight = 9999999;
    
    connect (this, &QQuickWindow::widthChanged, this, &ApplicationWindow::actualWidthChanged);
    connect (this, &QQuickWindow::heightChanged, this, &ApplicationWindow::actualHeightChanged);
    connect (this, &QQuickWindow::xChanged, this, &ApplicationWindow::actualXChanged);
    connect (this, &QQuickWindow::yChanged, this, &ApplicationWindow::actualYChanged);
    connect (this, &QQuickWindow::xChanged, this, &ApplicationWindow::onActualXChanged);
    connect (this, &QQuickWindow::yChanged, this, &ApplicationWindow::onActualYChanged);
    connect (contentItem (), &QQuickItem::xChanged, this, &ApplicationWindow::xChanged);
    connect (contentItem (), &QQuickItem::yChanged, this, &ApplicationWindow::yChanged);
    
    mouse_timer = new QTimer(this);
    connect (mouse_timer, SIGNAL(timeout()), SLOT(onDesktopPosChanged()));//连接定时器
    mouse_timer->start (20);//启动定时器，用来定时判断鼠标位置是否改变
    
}

bool ApplicationWindow::noNotifyIcon() const
{
    return m_noNotifyIcon;
}

int ApplicationWindow::width() const
{
    return m_width;
}

int ApplicationWindow::height() const
{
    return m_height;
}

int ApplicationWindow::actualWidth() const
{
    return QQuickWindow::width ();
}

int ApplicationWindow::actualHeight() const
{
    return QQuickWindow::height ();
}

bool ApplicationWindow::windowActive() const
{
    return m_windowActive;
}

int ApplicationWindow::x() const
{
    return QQuickWindow::x ()+contentItem ()->x ();
}

int ApplicationWindow::y() const
{
    return QQuickWindow::y ()+contentItem ()->y ();
}

int ApplicationWindow::actualX() const
{
    return QQuickWindow::x ();
}

int ApplicationWindow::actualY() const
{
    return QQuickWindow::y ();
}

int ApplicationWindow::minimumWidth() const
{
    return m_minimumWidth;
}

int ApplicationWindow::minimumHeight() const
{
    return m_minimumHeight;
}

int ApplicationWindow::maximumWidth() const
{
    return m_maximumWidth;
}

int ApplicationWindow::maximumHeight() const
{
    return m_maximumHeight;
}

bool ApplicationWindow::topHint() const
{
    return m_topHint;
}

bool ApplicationWindow::mousePenetrate() const
{
    return m_mousePenetrate;
}

QUrl ApplicationWindow::windowIcon()
{
    return m_windowIcon;
}

void ApplicationWindow::setWindowIcon(QUrl icon)
{
    if( icon!=m_windowIcon ){
        QString str = icon.toString ();
        if( str.mid (0, 3) == "qrc")
            str = str.mid (3, str.count ()-3);
        setIcon (QIcon(str));
        m_windowIcon = icon;
        emit windowIconChanged ();
    }
}

void ApplicationWindow::setWindowActive(bool arg)
{
    if(arg!=m_windowActive){
        m_windowActive = arg;
        emit windowActiveChanged (arg);
    }
}

void ApplicationWindow::focusInEvent(QFocusEvent *ev)
{
    QQuickWindow::focusInEvent(ev);
    setWindowActive (true);
}

void ApplicationWindow::focusOutEvent(QFocusEvent *ev)
{
    QQuickWindow::focusOutEvent (ev);
    setWindowActive (false);
}

void ApplicationWindow::onActualXChanged()
{
    emit xChanged();
}

void ApplicationWindow::onActualYChanged()
{
    emit yChanged();
}

void ApplicationWindow::onDesktopPosChanged()
{
    emit mouseDesktopPosChanged (QCursor::pos ());
}

bool ApplicationWindow::noBorder()
{
    return m_noBorder;
}

void ApplicationWindow::setNoBorder(bool isNoBroder)
{
    if( isNoBroder!=m_noBorder ) {
        m_noBorder = isNoBroder;
        if(!isVisible ()) {
            if( isNoBroder )
                setFlags (flags ()|Qt::FramelessWindowHint);
            else
                setFlags (flags ()&~Qt::FramelessWindowHint);
        }else{
            if( isNoBroder )
                setFlags (flags ()|Qt::FramelessWindowHint);
            else
                setFlags (flags ()&~Qt::FramelessWindowHint);
        }
        
        emit noBorderIconChanged();
    }
}

ApplicationWindow::WindowStatus ApplicationWindow::windowStatus()
{
    return m_windowStatus;
}

void ApplicationWindow::setWindowStatus(ApplicationWindow::WindowStatus new_status)
{
    if( new_status!=m_windowStatus ) {
        if( new_status == BerthPrepare&&m_windowStatus!=StopCenter ) {
            setTopHint (old_topHint);
        }else if(new_status!=StopCenter&&new_status!=BerthPrepare){
            old_topHint = topHint ();
            setTopHint (true);//设置窗口为最前端
        }
        m_windowStatus = new_status;
        emit windowStatusChanged ();
    }
}

int ApplicationWindow::borderLeft()
{
#ifdef Q_OS_WIN
    return 0;
#elif defined(Q_OS_MACX)
    return 0;
#elif defined(Q_OS_LINUX)
    return 65;
#else
    return 0;
#endif
}

int ApplicationWindow::borderRight()
{
#ifdef Q_OS_WIN
    return QApplication::screens ()[0]->size ().width ();
#elif defined(Q_OS_MACX)
    return QApplication::screens ()[0]->size ().width ();
#elif defined(Q_OS_LINUX)
    return QApplication::screens ()[0]->size ().width ();
#else
    return QApplication::screens ()[0]->size ().width ();
#endif
}

int ApplicationWindow::borderTop()
{
#ifdef Q_OS_WIN
    return 0;
#elif defined(Q_OS_OSX)
    return 0;
#elif defined(Q_OS_LINUX)
    return 25;
#else
    return 0;
#endif
}


void ApplicationWindow::setTopHint(bool arg)
{
    if (m_topHint != arg) {
        m_topHint = arg;
        if( arg ){
            setFlags (flags ()|Qt::WindowStaysOnTopHint);
#ifdef Q_OS_LINUX
            if(isVisible()){
                //setVisible(false);
                //setVisible(true);
                close();
                show();
            }
#endif
        }else{
            setFlags (flags ()&~Qt::WindowStaysOnTopHint);
#ifdef Q_OS_LINUX
            if(isVisible()){
                close();
                show();
                //setVisible(false);
                //setVisible(true);
            }
#endif
#ifdef Q_OS_WIN
            SetWindowPos ((HWND)this->winId (),HWND_NOTOPMOST,0,0,width(),height(),SWP_NOSIZE|SWP_NOMOVE);
#endif
        }
        emit topHintChanged(arg);
    }
}

void ApplicationWindow::setNoNotifyIcon(bool arg)
{
    if ( m_noNotifyIcon != arg ) {
        m_noNotifyIcon = arg;
        if( arg )
            setFlags (flags ()&~Qt::Tool);
        else{
            setFlags (flags ()|Qt::Tool);
        }
        emit noNotifyIconChanged(arg);
    }
}

void ApplicationWindow::setWidth(int arg)
{
    if (m_width != arg&&arg<=maximumWidth ()&&arg>=minimumWidth ()) {
        m_width = arg;
        contentItem ()->setWidth (arg);
        emit widthChanged(arg);
    }
}

void ApplicationWindow::setHeight(int arg)
{
    if (m_height != arg&&arg<=maximumHeight ()&&arg>=minimumHeight ()) {
        m_height = arg;
        contentItem ()->setHeight (arg);
        emit heightChanged(arg);
    }
}

void ApplicationWindow::setActualWidth(int arg)
{
    QQuickWindow::setWidth (arg);
}

void ApplicationWindow::setActualHeight(int arg)
{
    QQuickWindow::setHeight (arg);
}

void ApplicationWindow::setX(int arg)
{
    QQuickWindow::setX (arg-contentItem ()->x ());
}

void ApplicationWindow::setY(int arg)
{
    QQuickWindow::setY (arg-contentItem ()->y ());
}

void ApplicationWindow::setActualX(int arg)
{
    QQuickWindow::setX (arg);
}

void ApplicationWindow::setActualY(int arg)
{
    QQuickWindow::setY (arg);
}

void ApplicationWindow::setMinimumWidth(int arg)
{
    if (m_minimumWidth != arg) {
        m_minimumWidth = arg;
        int temp = actualWidth ()-width();//算出真实宽和内容宽（不算阴影的宽）的差值
        QQuickWindow::setMinimumWidth (temp+arg);//设置真实宽的限制
        if(width()<arg){
            setWidth (arg);
        }
        emit minimumWidthChanged(arg);
    }
}

void ApplicationWindow::setMinimumHeight(int arg)
{
    if (m_minimumHeight != arg) {
        m_minimumHeight = arg;
        int temp = actualHeight ()-height();
        QQuickWindow::setMinimumHeight (temp+arg);//设置真实高的限制
        if(height()<arg){
            setHeight (arg);
        }
        emit minimumHeightChanged(arg);
    }
}

void ApplicationWindow::setMaximumWidth(int arg)
{
    if (m_maximumWidth != arg) {
        m_maximumWidth = arg;
        int temp = actualWidth ()-width();//算出真实宽和内容宽（不算阴影的宽）的差值
        QQuickWindow::setMinimumWidth (temp+arg);//设置真实宽的限制
        if(width()>arg){
            setWidth (arg);
        }
        emit maximumWidthChanged(arg);
    }
}

void ApplicationWindow::setMaximumHeight(int arg)
{
    if (m_maximumHeight != arg) {
        m_maximumHeight = arg;
        int temp = actualHeight ()-height();
        QQuickWindow::setMinimumHeight (temp+arg);//设置真实高的限制
        if(height()>arg){
            setHeight (arg);
        }
        emit maximumHeightChanged(arg);
    }
}

void ApplicationWindow::setMousePenetrate(bool arg)
{
    if (m_mousePenetrate != arg) {
        m_mousePenetrate = arg;
#ifdef Q_OS_LINUX
        if(arg){
            XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
                    0, NULL, 0, ShapeSet, YXBanded);
        }else{
            XRectangle* myrect = new XRectangle;
            myrect->x = 0;
            myrect->y = 0;
            myrect->width = actualWidth ();
            myrect->height = actualHeight ();
            XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0,
                    0, myrect, 1, ShapeSet, YXBanded);
        }
#elif defined(Q_OS_OSX)
        qDebug()<<"mac os暂不支持鼠标穿透";
#elif defined(Q_OS_WIN)
        HWND my_hwnd = (HWND)this->winId ();
        if(arg){
            SetWindowLong(my_hwnd, GWL_EXSTYLE,
                         GetWindowLong(my_hwnd, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
        }else{
            SetWindowLong(my_hwnd, GWL_EXSTYLE,
                         GetWindowLong(my_hwnd, GWL_EXSTYLE)&(~WS_EX_TRANSPARENT));
        }
#endif
        emit mousePenetrateChanged(arg);
    }
}

void ApplicationWindow::close()
{
    emit closeing ();
    QQuickWindow::close ();
}

void ApplicationWindow::deleteWindow()
{
    deleteLater ();//销毁自己
}

