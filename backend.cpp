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
        if (QFile::copy(sourcePath, destPath))
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
        QDir destDir(destPath);

        if (!destDir.exists())
        {
            destDir.mkpath(".");
        }

        QDir sourceDir(sourcePath);
        QStringList fileList = sourceDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

        for (const QString &fileName : fileList)
        {
            QString sourceFilePath = sourceDir.filePath(fileName);
            QString destinationFilePath = destDir.filePath(fileName);

            moveFolder(sourceFilePath, destinationFilePath);
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

void Backend::deleteAllFolder(const QString &path)
{
    QDir dir(path);

    if (!dir.exists())
    {
        emit resultReady("Error: Folder does not exist.");
        return;
    }

    QFileInfoList fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo &fileInfo : fileList)
    {
        QString filePath = fileInfo.filePath();

        if (fileInfo.isFile())
        {
            QFile file(filePath);

            if (file.remove())
            {
                emit resultReady("File deleted successfully: " + fileInfo.fileName());
            }
            else
            {
                emit resultReady("Error: Unable to delete file: " + fileInfo.fileName());
            }
        }
        else if (fileInfo.isDir())
        {
            if (QDir().rmdir(filePath))
            {
                emit resultReady("Folder deleted successfully: " + fileInfo.fileName());
            }
            else
            {
                emit resultReady("Error: Unable to delete folder: " + fileInfo.fileName());
            }
        }
    }
}

void Backend::saveConfig(const QString &path, const QMap<QString, QVariant> &config)
{
    // create the file if it doesn't exist
    QFile file(path);
    // file.open(QIODevice::WriteOnly | QIODevice::Text);

    QJsonObject obj;
}

void Backend::loadConfig(const QString &path)
{
    QFile file(path);
    /*
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
                for (const QString &key : obj.keys())
                {
                    QJsonValue value = obj.value(key);

                    if (value.isBool())
                    {
                        configOptions.insert(key, value.toBool());
                    }
                    else if (value.isString())
                    {
                        configOptions.insert(key, value.toString());
                    }
                }
            }
            else
            {
                emit resultReady("Error: Unable to open file for reading.");
            }
        } */
}

void Backend::startExe(const char *path, const char *name, const char *args)
{
    // command: "cd "C:\Program Files (x86)\Steam\steamapps\common\PAYDAY 2" && start payday2_win32_release.exe"

    std::string command("cd ");
    // add the path
    command += std::string(path);
    command += std::string(" && start ");
    // add the name
    command += std::string(name);
    // add the args
    command += std::string(args);
    system(command.c_str());
}
