#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QJsoNObject>
#include <QJsonDocument>
#include <QIoDevice>
#include <QProcess>
#include <string>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    void moveFolder(const QString &sourcePath, const QString &destPath);
    void moveAllFiles(const QString &sourcePath, const QString &destPath);
    void deleteAllFolder(const QString &path);
    void saveConfig(const QString &path, const QMap<QString, QVariant> &config);
    void loadConfig(const QString &path);
    void startExe(const char *path, const char *name, const char *args);

signals:
    void resultReady(const QString &result);
    void config(const QString &result);
};

#endif // BACKEND_H
