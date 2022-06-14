/*****************************************************************************
* gta5hla GTA V Hardlinking Assistant
* Copyright (C) 2022 Syping
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* This software is provided as-is, no warranties are given to you, we are not
* responsible for anything with use of the software, you are self responsible.
*****************************************************************************/

// Qt includes
#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QMap>
#include <QSettings>
#include <QStringList>

// gta5hla includes
#include "HardlinkAssistant.h"

// Windows includes
#ifdef Q_OS_WIN
#include <windows.h>
#endif

HardlinkAssistant::HardlinkAssistant(QObject *parent) : QObject(parent)
{
    findPaths();
    findMasterGame();
}

void HardlinkAssistant::findPaths()
{
#ifdef Q_OS_WIN
    QString wow6432node;
#ifdef _WIN64
    wow6432node = "\\WOW6432Node";
#endif

    // Social Club
    QSettings registrySettingsSocialClub(QString("HKEY_LOCAL_MACHINE\\SOFTWARE%1").arg(wow6432node), QSettings::NativeFormat);
    QString installFolderSocialClub = registrySettingsSocialClub.value("Microsoft/Windows/CurrentVersion/Uninstall/{5EFC6C07-6B87-43FC-9524-F9E967241741}/InstallLocation", "").toString();
    if (QFile::exists(installFolderSocialClub + "/GTA5.exe")) {
        const QString installFolder = QDir(installFolderSocialClub).absolutePath();
        QJsonObject gameJsonData;
        gameJsonData["Path"] = installFolder;
        gameJsonData["Version"] = getGameVersion(installFolder);
        gameJsonData["Files"] = QJsonArray::fromStringList(getGameFiles(installFolder));
        m_jsonData["SocialClub"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "SC 1" << installFolder;
        qDebug() << "SC 1" << getGameVersion(installFolder);
        qDebug() << "SC 1" << getGameFiles(installFolder);
#endif
    }
    else {
        installFolderSocialClub = registrySettingsSocialClub.value("Rockstar Games/Grand Theft Auto V/InstallFolder", "").toString();
        if (QFile::exists(installFolderSocialClub + "/GTA5.exe")) {
            const QString installFolder = QDir(installFolderSocialClub).absolutePath();
            QJsonObject gameJsonData;
            gameJsonData["Path"] = installFolder;
            gameJsonData["Version"] = getGameVersion(installFolder);
            gameJsonData["Files"] = QJsonArray::fromStringList(getGameFiles(installFolder));
            m_jsonData["SocialClub"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
            qDebug() << "SC 2" << installFolder;
            qDebug() << "SC 2" << getGameVersion(installFolder);
            qDebug() << "SC 2" << getGameFiles(installFolder);
#endif
        }
    }

    // Steam
    QSettings registrySettingsSteam(QString("HKEY_LOCAL_MACHINE\\SOFTWARE%1\\Rockstar Games\\GTAV").arg(wow6432node), QSettings::NativeFormat);
    QString installFolderSteam = registrySettingsSteam.value("InstallFolderSteam", "").toString();
    if (installFolderSteam.endsWith("\\GTAV")) {
        installFolderSteam = installFolderSteam.remove(installFolderSteam.length() - 5, 5);
    }
    if (QFile::exists(installFolderSteam + "/GTA5.exe")) {
        const QString installFolder = QDir(installFolderSteam).absolutePath();
        QJsonObject gameJsonData;
        gameJsonData["Path"] = installFolder;
        gameJsonData["Version"] = getGameVersion(installFolder);
        gameJsonData["Files"] = QJsonArray::fromStringList(getGameFiles(installFolder));
        m_jsonData["Steam"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "Steam" << installFolder;
        qDebug() << "Steam" << getGameVersion(installFolder);
        qDebug() << "Steam" << getGameFiles(installFolder);
#endif
    }

    // Epic Games
    QSettings registrySettingsEpicGames(QString("HKEY_LOCAL_MACHINE\\SOFTWARE%1\\Rockstar Games\\Grand Theft Auto V").arg(wow6432node), QSettings::NativeFormat);
    QString installFolderEpicGames = registrySettingsEpicGames.value("InstallFolderEpic", "").toString();
    if (QFile::exists(installFolderEpicGames + "/GTA5.exe")) {
        const QString installFolder = QDir(installFolderEpicGames).absolutePath();
        QJsonObject gameJsonData;
        gameJsonData["Path"] = installFolder;
        gameJsonData["Version"] = getGameVersion(installFolder);
        gameJsonData["Files"] = QJsonArray::fromStringList(getGameFiles(installFolder));
        m_jsonData["EpicGames"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "EGS" << installFolder;
        qDebug() << "EGS" << getGameVersion(installFolder);
        qDebug() << "EGS" << getGameFiles(installFolder);
#endif
    }
#endif
}

void HardlinkAssistant::findMasterGame()
{
    QMap<QString, QStringList> gameVersionMap;
    QJsonValue gameJsonData = m_jsonData.value("SocialClub");
    if (gameJsonData.isObject()) {
        const QString gameVersion = gameJsonData.toObject().value("Version").toString();
        if (!gameVersion.isNull()) {
            QStringList gameList = gameVersionMap.value(gameVersion, QStringList());
            gameVersionMap[gameVersion] = gameList << "SocialClub";
        }
    }
    gameJsonData = m_jsonData.value("Steam");
    if (gameJsonData.isObject()) {
        const QString gameVersion = gameJsonData.toObject().value("Version").toString();
        if (!gameVersion.isNull()) {
            QStringList gameList = gameVersionMap.value(gameVersion, QStringList());
            gameVersionMap[gameVersion] = gameList << "Steam";
        }
    }
    gameJsonData = m_jsonData.value("EpicGames");
    if (gameJsonData.isObject()) {
        const QString gameVersion = gameJsonData.toObject().value("Version").toString();
        if (!gameVersion.isNull()) {
            QStringList gameList = gameVersionMap.value(gameVersion, QStringList());
            gameVersionMap[gameVersion] = gameList << "EpicGames";
        }
    }

    if (gameVersionMap.count() < 2)
        return;

    QStringList gameVersionList = gameVersionMap.keys();
    std::sort(gameVersionList.rbegin(), gameVersionList.rend());

    const QString lastGameVersion = gameVersionList.first();
    const QStringList masterGameList = gameVersionMap.value(lastGameVersion);
    m_jsonData["MasterGames"] = QJsonArray::fromStringList(masterGameList);
    m_masterGame = masterGameList.first();

#ifdef GTA5HLA_DEBUG
    qDebug() << "Master Game" << m_masterGame;
#endif
}

bool HardlinkAssistant::linkFile(const QString &masterGamePath, const QString &slaveGamePath, const QString &gameFile)
{
    const QString srcLinkFileName = masterGamePath + "/" + gameFile;
    const QString destGameFileName = slaveGamePath + "/" + gameFile;
    const QString destLinkFileName = destGameFileName + ".link";
    const QString destUnlinkFileName = destGameFileName + ".unlink";
#ifdef Q_OS_WIN
    bool ok = CreateHardLinkW(destLinkFileName.toStdWString().data(), srcLinkFileName.toStdWString().data(), NULL);
#else
    bool ok = link(srcLinkFileName.toUtf8().data(), destLinkFileName.toUtf8().data());
#endif
    if (ok) {
        if (QFile::rename(destGameFileName, destUnlinkFileName)) {
            if (QFile::rename(destLinkFileName, destGameFileName)) {
                if (!QFile::remove(destUnlinkFileName)) {
                    emit logUpdated(tr("Warning: Failed to cleanup %1").arg(gameFile));
                }
                emit logUpdated(tr("Linked: %1").arg(gameFile));
                return true;
            }
            else {
                emit logUpdated(tr("Error: Failed to replace %1").arg(gameFile));
                return false;
            }
        }
        else {
            emit logUpdated(tr("Error: Failed to secure %1").arg(gameFile));
            return false;
        }
    }
    else {
        emit logUpdated(tr("Error: Failed to link %1").arg(gameFile));
        return false;
    }
}

bool HardlinkAssistant::linkGame(const QString &masterGamePath, const QString &slaveGamePath, const QStringList &masterGameFiles)
{
    if (QFile::exists(masterGamePath + "/GTA5.exe") && QFile::exists(slaveGamePath + "/GTA5.exe")) {
        int currentProgress = 0;
        emit linkProgressCurrentUpdated(currentProgress);
        emit linkProgressMaxUpdated(masterGameFiles.count());

        const QRegularExpression updateRegExp("(?i)\\/update\\d*\\.rpf$");

        for (const QString &gameFile : masterGameFiles) {
            if (!QFile::exists(slaveGamePath + "/" + gameFile)) {
                emit logUpdated(tr("Skipped: %1").arg(gameFile));
            }
            else if (gameFile.endsWith(".dat", Qt::CaseInsensitive) || gameFile.endsWith(".dll", Qt::CaseInsensitive) || gameFile.contains(updateRegExp)) {
                emit logUpdated(tr("Hashing 1/2 %1...").arg(gameFile));

                QByteArray masterGameHash, slaveGameHash;
                QFile gameFileHandle;
                gameFileHandle.setFileName(masterGamePath + "/" + gameFile);
                if (gameFileHandle.open(QIODevice::ReadOnly)) {
                    QCryptographicHash cryptoHash(QCryptographicHash::Sha256);
                    if (cryptoHash.addData(&gameFileHandle)) {
                        masterGameHash = cryptoHash.result();
                        gameFileHandle.close();

                        emit logUpdated(tr("Hashing 2/2 %1...").arg(gameFile));

                        gameFileHandle.setFileName(slaveGamePath + "/" + gameFile);
                        if (gameFileHandle.open(QIODevice::ReadOnly)) {
                            QCryptographicHash cryptoHash(QCryptographicHash::Sha256);
                            if (cryptoHash.addData(&gameFileHandle)) {
                                slaveGameHash = cryptoHash.result();
                            }
                            gameFileHandle.close();
                            if (masterGameHash.isEmpty() || slaveGameHash.isEmpty()) {
                                emit logUpdated(tr("Error: Failed to hash %1").arg(gameFile));
                                return false;
                            }
                            else if (masterGameHash == slaveGameHash) {
                                emit logUpdated(tr("Linking %1...").arg(gameFile));

                                bool ok = linkFile(masterGamePath, slaveGamePath, gameFile);
                                if (!ok)
                                    return false;
                            }
                            else {
                                emit logUpdated(tr("Unmatched: %1").arg(gameFile));
                            }
                        }
                        else {
                            emit logUpdated(tr("Error: Failed to hash %1").arg(gameFile));
                            return false;
                        }
                    }
                    else {
                        gameFileHandle.close();
                        emit logUpdated(tr("Error: Failed to hash %1").arg(gameFile));
                        return false;
                    }
                }
                else {
                    emit logUpdated(tr("Error: Failed to hash %1").arg(gameFile));
                    return false;
                }
            }
            else {
                emit logUpdated(tr("Linking %1...").arg(gameFile));

                bool ok = linkFile(masterGamePath, slaveGamePath, gameFile);
                if (!ok)
                    return false;
            }

            currentProgress++;
            emit linkProgressCurrentUpdated(currentProgress);
        }

        return true;
    }
    else {
        return false;
    }
}

const QString& HardlinkAssistant::getMasterGame()
{
    return m_masterGame;
}

const QJsonObject& HardlinkAssistant::getJsonData()
{
    return m_jsonData;
}

const QStringList HardlinkAssistant::getGameFiles(const QString &dirPath)
{
    if (QFile::exists(dirPath + "/GTA5.exe")) {
        QStringList gameFiles;
        const qsizetype prefixLength = dirPath.length() + 1;
        QDirIterator mainDirIterator(dirPath, {"*.dll", "*.rpf"}, QDir::Files);
        while (mainDirIterator.hasNext()) {
            QString gameFile = mainDirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        QDirIterator updateDirIterator(dirPath + "/update", {"*.rpf"}, QDir::Files);
        while (updateDirIterator.hasNext()) {
            QString gameFile = updateDirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        QDirIterator updatex64DirIterator(dirPath + "/update/x64", {"*.dat"}, QDir::Files);
        while (updatex64DirIterator.hasNext()) {
            QString gameFile = updatex64DirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        QDirIterator x64DirIterator(dirPath + "/x64", {"*.dat"}, QDir::Files);
        while (x64DirIterator.hasNext()) {
            QString gameFile = x64DirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        QDirIterator dlcpacksDirIterator(dirPath + "/update/x64/dlcpacks", QDir::Dirs);
        while (dlcpacksDirIterator.hasNext()) {
            QDirIterator dlcpackDirIterator(dlcpacksDirIterator.next(), {"*.rpf"}, QDir::Files);
            while (dlcpackDirIterator.hasNext()) {
                QString gameFile = dlcpackDirIterator.next();
                gameFiles << gameFile.remove(0, prefixLength);
            }
        }
        QDirIterator audioDirIterator(dirPath + "/x64/audio", {"*.rpf"}, QDir::Files);
        while (audioDirIterator.hasNext()) {
            QString gameFile = audioDirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        QDirIterator sfxDirIterator(dirPath + "/x64/audio/sfx", {"*.rpf"}, QDir::Files);
        while (sfxDirIterator.hasNext()) {
            QString gameFile = sfxDirIterator.next();
            gameFiles << gameFile.remove(0, prefixLength);
        }
        return gameFiles;
    }
    return QStringList();
}

const QString HardlinkAssistant::getGameVersion(const QString &dirPath)
{
#ifdef Q_OS_WIN
    QFile gameExecutable(dirPath + "/GTA5.exe");
    if (gameExecutable.exists()) {
        return getExecutableVersion(gameExecutable.fileName());
    }
#endif
    return QString();
}

const QString HardlinkAssistant::getExecutableVersion(const QString &filePath)
{
#ifdef Q_OS_WIN
    DWORD dwHandle, dwSize;
    dwSize = GetFileVersionInfoSizeW(filePath.toStdWString().data(), &dwHandle);
    if (dwSize == 0)
        return QString();
    LPVOID buffer[dwSize];
    if (!GetFileVersionInfoW(filePath.toStdWString().data(), dwHandle, dwSize, &buffer)) {
        return QString();
    }
    VS_FIXEDFILEINFO *vsFileInfo;
    dwSize = sizeof(VS_FIXEDFILEINFO);
    if (!VerQueryValueW(&buffer, L"\\", (LPVOID*)&vsFileInfo, (PUINT)&dwSize)) {
        return QString();
    }
    char version[128];
    const int length = sprintf(version, "%hu.%hu.%hu.%hu",
                         HIWORD(vsFileInfo->dwFileVersionMS),
                         LOWORD(vsFileInfo->dwFileVersionMS),
                         HIWORD(vsFileInfo->dwFileVersionLS),
                         LOWORD(vsFileInfo->dwFileVersionLS));
    return QString::fromLatin1(version, length);
#else
    return QString();
#endif
}

void HardlinkAssistant::setMasterGame(const QString &masterGame)
{
    m_masterGame = masterGame;
}

void HardlinkAssistant::setJsonData(QJsonObject jsonData)
{
    m_jsonData = jsonData;
}
