#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    m_configPath = "config.json";
    m_debugFile.setFileName("debug.txt");
    m_debugFile.open(QIODevice::WriteOnly | QIODevice::Text);
}

Backend::~Backend()
{
    m_debugFile.close();
}

void Backend::moveFolder(const QString &sourcePath, const QString &destPath)
{
    QFileInfo sourceInfo(sourcePath);

    if (!sourceInfo.exists())
    {
        debug("Error: Source folder or file does not exist.");
        return;
    }

    if (sourceInfo.isFile())
    {
        if (QFile::copy(sourcePath, destPath))
        {
            debug("File moved successfully: " + sourceInfo.fileName());
        }
        else
        {
            debug("Error: Unable to move file: " + sourceInfo.fileName());
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
        debug("Error: Folder does not exist.");
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
                debug("File deleted successfully: " + fileInfo.fileName());
            }
            else
            {
                debug("Error: Unable to delete file: " + fileInfo.fileName());
            }
        }
        else if (fileInfo.isDir())
        {
            QDir subDir(filePath);
            if (subDir.removeRecursively())
            {
                debug("Folder deleted successfully: " + fileInfo.fileName());
            }
            else
            {
                debug("Error: Unable to delete folder at path: " + filePath);
            }
        }
    }
}

void Backend::saveConfig(const QJsonObject &config)
{
    QJsonObject configObject = config;
    // add a new subobject to the config object
    QJsonObject hashObject;
    // if selected profile is rush
    if (config.value("profile") == "rush")
    {
        // add the hash of the mods folder
        hashObject.insert("modsHash", hashDir("mods_rush"));
        // add the hash of the mod_overrides folder
        hashObject.insert("modOverridesHash", hashDir("mod_overrides_rush"));
        configObject.insert("hashes_rush", hashObject);
    }
    else if (config.value("profile") == "normal")
    {
        // add the hash of the mods folder
        hashObject.insert("modsHash", hashDir("mods"));
        // add the hash of the mod_overrides folder
        hashObject.insert("modOverridesHash", hashDir("mod_overrides"));
        configObject.insert("hashes_normal", hashObject);
    }

    // create the file if it doesn't exist
    QFile file(m_configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(QJsonDocument(configObject).toJson());
        file.close();
        debug("Config saved successfully.");
    }
    else
    {
        debug("Error: Unable to open file for writing.");
    }
}

void Backend::loadConfig()
{
    QFile file(m_configPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray data = file.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject config = jsonDoc.object();
            emit configReady(config);
            if (config.contains("hashes_rush") && config.value("hashes_rush").toObject().value("modsHash").toString() != hashDir("../mods"))
            {
                deleteAllFolder("mods_rush");
                moveAllFiles("../mods", "mods_rush");

                debug("Mods folder updated for rush profile.");
            }
            if (config.contains("hashes_rush") && config.value("hashes_rush").toObject().value("modOverridesHash").toString() != hashDir("../assets/mod_overrides"))
            {
                deleteAllFolder("mod_overrides_rush");
                moveAllFiles("../assets/mod_overrides", "mod_overrides_rush");

                debug("Mod overrides folder updated for rush profile.");
            }
            if (config.contains("hashes_normal") && config.value("hashes_normal").toObject().value("modsHash").toString() != hashDir("mods"))
            {
                deleteAllFolder("mods_normal");
                moveAllFiles("mods", "mods_normal");

                debug("Mods folder updated for normal profile.");
            }
            if (config.contains("hashes_normal") && config.value("hashes_normal").toObject().value("modOverridesHash").toString() != hashDir("mod_overrides"))
            {
                deleteAllFolder("mod_overrides_normal");
                moveAllFiles("mod_overrides", "mod_overrides_normal");

                debug("Mod overrides folder updated for normal profile.");
            }
        }
        debug("Config loaded successfully.");
    }
    else
    {
        debug("Error: Unable to open file for reading.");
    }
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

void Backend::addToHash(const QFileInfo &fileInfo, QCryptographicHash &hash)
{
    QDir dir(fileInfo.absoluteFilePath());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    foreach (const QFileInfo &fileInfo, fileList)
    {
        if (fileInfo.isFile())
        {
            QFile file(fileInfo.absoluteFilePath());
            if (!file.open(QIODevice::ReadOnly))
            {
                debug("Error: Unable to open file for reading.");
                continue;
            }
            else
            {
                hash.addData(file.readAll());
                file.close();
            }
        }
        else if (fileInfo.isDir())
        {
            addToHash(fileInfo, hash);
            continue;
        }
    }
}

QString Backend::hashDir(const QString &path)
{
    QFileInfo fileInfo(path);
    QString hash;
    QCryptographicHash crypto(QCryptographicHash::Sha1);
    addToHash(fileInfo, crypto);
    hash = crypto.result().toHex();

    return hash;
}

void Backend::debug(const QString &msg)
{
    QTextStream out(&m_debugFile);
    out << msg << "\n";
}