#include "mainwindow.h"
#include "backend.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile f(":qdarkstyle/dark/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
        printf("Stylesheet successfully loaded\n");
    }

    Backend backend;
    MainWindow w(&backend);
    w.show();

    QObject::connect(&backend, &Backend::resultReady, [](const QString &result)
                     { qDebug() << result; });

    return a.exec();
}
