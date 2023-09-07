#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{
}

void Backend::moveFolder(const QString &sourcePath, const QString &destPath)
{
    QFileInfo sourceInfo(sourcePath);

    if (!sourceInfo.exists())
    {
        emit resultReady("Error: Source folder or file does not exist.");
        return;
    }

    if (sourceInfo.isFile())
    {
        if (QFile::rename(sourcePath, destPath))
        {
            emit resultReady("File moved successfully: " + sourceInfo.fileName());
        }
        else
        {
            emit resultReady("Error: Unable to move file: " + sourceInfo.fileName());
        }
    }
    else if (sourceInfo.isDir())
    {
        QDir dir;

        if (dir.rename(sourcePath, destPath))
        {
            emit resultReady("Folder moved successfully.");
        }
        else
        {
            emit resultReady("Error: Unable to move folder");
        }
    }
}

void Backend::moveAllFiles(const QString &sourcePath, const QString &destPath)
{
    QDir sourceDir(sourcePath);
    QDir destDir(destPath);

    if (!sourceDir.exists())
    {
        sourceDir.mkpath(".");

        return;
    }

    QStringList fileList = sourceDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    for (const QString &fileName : fileList)
    {
        QString sourceFilePath = sourceDir.filePath(fileName);
        QString destinationFilePath = destDir.filePath(fileName);

        moveFolder(sourceFilePath, destinationFilePath);
    }
}

void Backend::saveConfig(const QString &path, const QString &config)
{
    // create the file if it doesn't exist
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream(&file);
    stream << config;
    file.close();
}

void Backend::loadConfig(const QString &path)
{
    QFile file(path);

    if (file.open(QIoDevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));

        if (!doc.isNull())
        {
            QJsonObject obj = doc.object();

            // Dictionnary for config options that can store either a bool or a string
            QMap<QString, QVariant> configOptions;

            // Get config options
            configOptions["matchmaking"] = obj["matchmaking"].toString();
        }
        else
        {
            emit resultReady("Error: Unable to open file for reading.");
        }
    }