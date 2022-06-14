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

#ifndef HARDLINKASSISTANT_H
#define HARDLINKASSISTANT_H

// Qt includes
#include <QApplication>
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class HardlinkAssistant : public QObject
{
    Q_OBJECT
public:
    explicit HardlinkAssistant(QObject *parent = nullptr);
    void findPaths();
    void findMasterGame();
    bool linkFile(const QString &masterGamePath, const QString &slaveGamePath, const QString &gameFile);
    bool linkGame(const QString &masterGamePath, const QString &slaveGamePath, const QStringList &masterGameFiles);
    const QString& getMasterGame();
    const QJsonObject& getJsonData();
    static const QStringList getGameFiles(const QString &dirPath);
    static const QString getGameVersion(const QString &dirPath);
    static const QString getExecutableVersion(const QString &filePath);
    void setMasterGame(const QString &masterGame);
    void setJsonData(QJsonObject jsonData);

signals:
    void linkProgressCurrentUpdated(int current);
    void linkProgressMaxUpdated(int max);
    void logUpdated(const QString &text);

private:
    QString m_masterGame;
    QJsonObject m_jsonData;
};

#endif // HARDLINKASSISTANT_H
