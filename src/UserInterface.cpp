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
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

// gta5hla includes
#include "HardlinkThread.h"
#include "ProgressDialog.h"
#include "UserInterface.h"
#include "ui_UserInterface.h"

UserInterface::UserInterface(HardlinkAssistant *hla, QWidget *parent) : QDialog(parent),
    hla(hla), ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    QObject::connect(hla, &HardlinkAssistant::logUpdated, this, &UserInterface::logUpdated);

    QJsonValue gameObjectVal = hla->getJsonData().value("SocialClub");
    if (gameObjectVal.isObject()) {
        QJsonObject gameObject = gameObjectVal.toObject();
        const QString gamePath = gameObject.value("Path").toString();
        ui->txtSocialClub->setText(gamePath);
        const QString gameVersion = gameObject.value("Version").toString();
        ui->labSocialClub->setText(tr("Social Club (%1):").arg(gameVersion));
    }

    gameObjectVal = hla->getJsonData().value("Steam");
    if (gameObjectVal.isObject()) {
        QJsonObject gameObject = gameObjectVal.toObject();
        const QString gamePath = gameObject.value("Path").toString();
        ui->txtSteam->setText(gamePath);
        const QString gameVersion = gameObject.value("Version").toString();
        ui->labSteam->setText(tr("Steam (%1):").arg(gameVersion));
    }

    gameObjectVal = hla->getJsonData().value("EpicGames");
    if (gameObjectVal.isObject()) {
        QJsonObject gameObject = gameObjectVal.toObject();
        const QString gamePath = gameObject.value("Path").toString();
        ui->txtEpicGames->setText(gamePath);
        const QString gameVersion = gameObject.value("Version").toString();
        ui->labEpicGames->setText(tr("Epic Games (%1):").arg(gameVersion));
    }

    ui->btnLink->setFocus();
}

void UserInterface::logUpdated(const QString &text)
{
#ifdef GTA5HLA_DEBUG
    qDebug() << text;
#endif
}

void UserInterface::on_btnSocialClub_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Social Club folder"));
    if (!gamePath.isEmpty()) {
        if (QFile::exists(gamePath + "/GTA5.exe")) {
            const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

            QJsonObject gameJsonData;
            gameJsonData["Path"] = gamePath;
            gameJsonData["Version"] = gameVersion;
            gameJsonData["Files"] = QJsonArray::fromStringList(HardlinkAssistant::getGameFiles(gamePath));

            ui->txtSocialClub->setText(gamePath);
            ui->labSocialClub->setText(tr("Social Club (%1):").arg(gameVersion));

            QJsonObject jsonData = hla->getJsonData();
            jsonData["SocialClub"] = gameJsonData;
            hla->setJsonData(jsonData);
            hla->findMasterGame();
        }
        else {
            QMessageBox::warning(this, tr("Select GTA V Social Club folder"), "The selected folder is not a valid GTA V folder!");
        }
    }
}

void UserInterface::on_btnSteam_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Steam folder"));
    if (!gamePath.isEmpty()) {
        if (QFile::exists(gamePath + "/GTA5.exe")) {
            const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

            QJsonObject gameJsonData;
            gameJsonData["Path"] = gamePath;
            gameJsonData["Version"] = gameVersion;
            gameJsonData["Files"] = QJsonArray::fromStringList(HardlinkAssistant::getGameFiles(gamePath));

            ui->txtSteam->setText(gamePath);
            ui->labSteam->setText(tr("Steam (%1):").arg(gameVersion));

            QJsonObject jsonData = hla->getJsonData();
            jsonData["Steam"] = gameJsonData;
            hla->setJsonData(jsonData);
            hla->findMasterGame();
        }
        else {
            QMessageBox::warning(this, tr("Select GTA V Steam folder"), "The selected folder is not a valid GTA V folder!");
        }
    }
}

void UserInterface::on_btnEpicGames_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Epic Games folder"));
    if (!gamePath.isEmpty()) {
        if (QFile::exists(gamePath + "/GTA5.exe")) {
            const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

            QJsonObject gameJsonData;
            gameJsonData["Path"] = gamePath;
            gameJsonData["Version"] = gameVersion;
            gameJsonData["Files"] = QJsonArray::fromStringList(HardlinkAssistant::getGameFiles(gamePath));

            ui->txtEpicGames->setText(gamePath);
            ui->labEpicGames->setText(tr("Epic Games (%1):").arg(gameVersion));

            QJsonObject jsonData = hla->getJsonData();
            jsonData["EpicGames"] = gameJsonData;
            hla->setJsonData(jsonData);
            hla->findMasterGame();
        }
        else {
            QMessageBox::warning(this, tr("Select GTA V Epic Games folder"), "The selected folder is not a valid GTA V folder!");
        }
    }
}

void UserInterface::on_btnLink_clicked()
{
    QStringList masterGameList;
    const QJsonArray masterGameArray = hla->getJsonData().value("MasterGames").toArray();
    for (const QJsonValue &masterGameVal : masterGameArray) {
        const QString masterGame = masterGameVal.toString();
        masterGameList << masterGame;
    }

    QString masterGame = hla->getMasterGame();
    if (masterGameList.count() > 1) {
        bool ok;
        masterGame = QInputDialog::getItem(this, "gta5hla - Main Game", "Select your Main Game", masterGameList, 0, false, &ok);

        if (!ok)
            return;

        hla->setMasterGame(masterGame);
    }

    if (masterGame.isEmpty()) {
        QMessageBox::warning(this, "gta5hla", "Can't find a Main Game to link to!");
        return;
    }

    QStringList gameList;
    if (hla->getJsonData().contains("SocialClub"))
        gameList << "SocialClub";
    if (hla->getJsonData().contains("Steam"))
        gameList << "Steam";
    if (hla->getJsonData().contains("EpicGames"))
        gameList << "EpicGames";

    QStringList slaveGameList = gameList;
    slaveGameList.removeAll(masterGame);

    QStringList masterGameFiles;
    const QString masterGamePath = hla->getJsonData().value(masterGame).toObject().value("Path").toString();
    const QJsonArray masterGameFilesArray = hla->getJsonData().value(masterGame).toObject().value("Files").toArray();
    for (const QJsonValue &masterGameFileVal : masterGameFilesArray) {
        masterGameFiles << masterGameFileVal.toString();
    }

    ProgressDialog progressDialog(hla, this);
    progressDialog.open();

    QStringList errorGameList;

    for (const QString &slaveGame : slaveGameList) {
        progressDialog.linkProgressCurrentUpdated(0);
        progressDialog.setLinkLabelText(tr("Linking: %1").arg(slaveGame));
        progressDialog.setWindowTitle(tr("gta5hla - Linking %1...").arg(slaveGame));

        QEventLoop loop;

        const QString slaveGamePath = hla->getJsonData().value(slaveGame).toObject().value("Path").toString();

        HardlinkThread hardlinkThread(hla, masterGamePath, slaveGamePath, masterGameFiles);
        QObject::connect(&hardlinkThread, &HardlinkThread::finished, this, [&]() {
            loop.exit();
        });
        hardlinkThread.start();

        loop.exec();

        bool ok = hardlinkThread.isOk();
        if (ok) {
            progressDialog.appendLog(tr("Game Linked: %1").arg(slaveGame));
        }
        else {
            progressDialog.appendLog(tr("Error: Linking %1 game failed").arg(slaveGame));
            errorGameList << slaveGame;
        }
    }

    if (errorGameList.count() != 0) {
        QMessageBox::critical(&progressDialog, "gta5hla", tr("Error: Linking %n game(s) failed!\n\nFailed game(s): %1", "", errorGameList.count()).arg(errorGameList.join(", ")));
    }

    close();
}

UserInterface::~UserInterface()
{
    delete ui;
}
