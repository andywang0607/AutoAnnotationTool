#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <labelcollector.h>
#include <labeldatamodel.h>
#include <cvmodule.h>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<LabelCollector>("LabelCollector", 1, 0, "LabelCollector");
    qmlRegisterType<LabelDataModel>("LabelDataModel", 1, 0, "LabelDataModel");
    qmlRegisterType<CVModule>("CVModule", 1, 0, "CVModule");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
