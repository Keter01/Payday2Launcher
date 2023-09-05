#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{

}

void Backend::moveFolder(const QString &sourcePath, const QString &destPath)
{
    QFileInfo sourceInfo(sourcePath);

    if (!sourceInfo.exists()) {
        emit resultReady("Error: Source folder or file does not exist.");
        return;
    }

    if (sourceInfo.isFile()) {
        if (QFile::rename(sourcePath, destPath)) {
            emit resultReady("File moved successfully: " + sourceInfo.fileName());
        } else {
            emit resultReady("Error: Unable to move file: " + sourceInfo.fileName());
        }
    } else if (sourceInfo.isDir()){
        QDir dir;

        if (dir.rename(sourcePath, destPath)) {
            emit resultReady("Folder moved successfully.");
        } else {
            emit resultReady("Error: Unable to move folder");
        }
    }
}

void Backend::moveAllFiles(const QString &sourcePath, const QString &destPath)
{
    QDir sourceDir(sourcePath);
    QDir destDir(destPath);

    if (!sourceDir.exists()) {
        sourceDir.mkpath(".");

        return;
    }

    QStringList fileList = sourceDir.entryList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    for (const QString &fileName : fileList) {
        QString sourceFilePath = sourceDir.filePath(fileName);
        QString destinationFilePath = destDir.filePath(fileName);

        moveFolder(sourceFilePath, destinationFilePath);
    }
}
