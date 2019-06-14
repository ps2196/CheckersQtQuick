#include <QGuiApplication>
//#include <QQuickView>
//#include <QQmlEngine>
//#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "checkersgame.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

//    QQuickView view;
//    CheckersGame game; game.newGame();
//    view.engine()->rootContext()->setContextProperty("game", &game);
//    view.engine()->rootContext()->setContextProperty("checkersBoard", game.board());
//    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
//    view.show();


    qmlRegisterType<CheckersGame>("CheckersGame", 1, 0, "CheckersGame");
    qmlRegisterType<CheckersBoard>("CheckersBoard", 1, 0, "CheckersBoard");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;


    return app.exec();
}
