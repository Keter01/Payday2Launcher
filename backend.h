#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QJsoNObject>
#include <QJsonDocument>
#include <QIoDevice>
#include <QProcess>
#include <QCryptographicHash>
#include <string>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();
    void moveFolder(const QString &sourcePath, const QString &destPath);
    void moveAllFiles(const QString &sourcePath, const QString &destPath);
    void deleteAllFolder(const QString &path);
    void saveConfig(const QJsonObject &config);
    void loadConfig();
    void startExe(const char *path, const char *name, const char *args);
    void addToHash(const QFileInfo &fileInfo, QCryptographicHash &hash);
    QString hashDir(const QString &path);
    void debug(const QString &msg);

signals:
    void resultReady(const QString &result);
    void configReady(const QJsonObject &config);

private:
    QString m_configPath;
    QFile m_debugFile;
};

#endif // BACKEND_H
