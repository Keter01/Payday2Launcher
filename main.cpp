#include "mainwindow.h"
#include "backend.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Backend backend;
    MainWindow w(&backend);
    w.show();

    QObject::connect(&backend, &Backend::resultReady, [](const QString &result) {
        qDebug() << result;
    });

    return a.exec();
}
