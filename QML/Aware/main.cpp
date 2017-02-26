#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "itunes/core/logme.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LogMe log("iYoona");
    log.info("Hi, I'm iYoona !");

    QQmlApplicationEngine engine;

#ifdef Q_USE_QML_RC
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
#else
    engine.load(QUrl::fromLocalFile(QGuiApplication::applicationDirPath() +
                                #if defined(Q_OS_WIN)
                                    QStringLiteral("")
                                #elif defined(Q_OS_MAC)
                                    QStringLiteral("/../../..")
                                #endif
                                + QStringLiteral("/../../QML/Aware/qml/main.qml")));
#endif

    return app.exec();
}
