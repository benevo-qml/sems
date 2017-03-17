#include <QApplication>
#include <QDesktopWidget>
#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QQuickView view;
#ifdef QML_IN_QRC_FILE
    view.setSource(QUrl("qrc:/qml/main.qml"));
#else
    view.setSource(QUrl::fromLocalFile(DAA_DIR + "/QML/iCIM/qml/main.qml"));
#endif
    view.rootContext()->setContextProperty("mainwindow", &view);
    view.setColor(QColor(Qt::transparent));

    int w = QApplication::desktop()->availableGeometry().width();
    int h = QApplication::desktop()->availableGeometry().height();
    view.setGeometry((w - 910 )/2, (h-610)/2, 925, 620);
    view.show();
    view.setFlags(Qt::Window|Qt::FramelessWindowHint); // should be placed after view.show（）

    return a.exec();
}
