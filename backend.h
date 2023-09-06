#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QFile>
#include <QDir>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    void moveFolder(const QString &sourcePath, const QString &destPath);
    void moveAllFiles(const QString &sourcePath, const QString &destPath);
    void saveConfig(const QString &path, const QString &config);
    void loadConfig(const QString &path);

signals:
    void resultReady(const QString &result);
    void config(const QString &result);
};

#endif // BACKEND_H
