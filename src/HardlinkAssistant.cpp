// Qt includes
#include <QDir>
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

HardlinkAssistant::HardlinkAssistant(QObject *parent) : QObject{parent}
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
        m_jsonData["SocialClub"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "SC 1" << installFolder;
        qDebug() << "SC 1" << getGameVersion(installFolder);
#endif
    }
    else {
        installFolderSocialClub = registrySettingsSocialClub.value("Rockstar Games/Grand Theft Auto V/InstallFolder", "").toString();
        if (QFile::exists(installFolderSocialClub + "/GTA5.exe")) {
            const QString installFolder = QDir(installFolderSocialClub).absolutePath();
            QJsonObject gameJsonData;
            gameJsonData["Path"] = installFolder;
            gameJsonData["Version"] = getGameVersion(installFolder);
            m_jsonData["SocialClub"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
            qDebug() << "SC 2" << installFolder;
            qDebug() << "SC 2" << getGameVersion(installFolder);
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
        m_jsonData["Steam"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "Steam" << installFolder;
        qDebug() << "Steam" << getGameVersion(installFolder);
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
        m_jsonData["EpicGames"] = gameJsonData;
#ifdef GTA5HLA_DEBUG
        qDebug() << "EGS" << installFolder;
        qDebug() << "EGS" << getGameVersion(installFolder);
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

const QString& HardlinkAssistant::getMasterGame()
{
    return m_masterGame;
}

const QJsonObject& HardlinkAssistant::getJsonData()
{
    return m_jsonData;
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
