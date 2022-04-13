#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <opencv_player_viewport.h>
#include <opencv_videoplayer.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("material");
    qmlRegisterType<OpenCV_Player_ViewPort>("app.OpenCV_Player_ViewPort", 1, 0 , "OpenCV_Player_ViewPort");
    qmlRegisterType<OpenCV_VideoPlayer>("app.OpenCV_VideoPlayer", 1, 0 , "OpenCV_VideoPlayer");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
