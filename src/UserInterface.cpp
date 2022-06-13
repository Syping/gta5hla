// Qt includes
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

// gta5hla includes
#include "UserInterface.h"
#include "ui_UserInterface.h"

UserInterface::UserInterface(HardlinkAssistant *hla, QWidget *parent) :
    QDialog(parent), hla(hla), ui(new Ui::UserInterface)
{
    ui->setupUi(this);

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

    ui->btnOk->setFocus();
}


void UserInterface::on_btnSocialClub_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Social Club folder"));
    if (!gamePath.isEmpty() && QFile::exists(gamePath + "/GTA5.exe")) {
        const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

        QJsonObject gameJsonData;
        gameJsonData["Path"] = gamePath;
        gameJsonData["Version"] = gameVersion;

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

void UserInterface::on_btnSteam_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Steam folder"));
    if (!gamePath.isEmpty() && QFile::exists(gamePath + "/GTA5.exe")) {
        const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

        QJsonObject gameJsonData;
        gameJsonData["Path"] = gamePath;
        gameJsonData["Version"] = gameVersion;

        ui->txtEpicGames->setText(gamePath);
        ui->labEpicGames->setText(tr("Epic Games (%1):").arg(gameVersion));

        QJsonObject jsonData = hla->getJsonData();
        jsonData["Steam"] = gameJsonData;
        hla->setJsonData(jsonData);
        hla->findMasterGame();
    }
    else {
        QMessageBox::warning(this, tr("Select GTA V Steam folder"), "The selected folder is not a valid GTA V folder!");
    }
}

void UserInterface::on_btnEpicGames_clicked()
{
    const QString gamePath = QFileDialog::getExistingDirectory(this, tr("Select GTA V Epic Games folder"));
    if (!gamePath.isEmpty() && QFile::exists(gamePath + "/GTA5.exe")) {
        const QString gameVersion = HardlinkAssistant::getGameVersion(gamePath);

        QJsonObject gameJsonData;
        gameJsonData["Path"] = gamePath;
        gameJsonData["Version"] = gameVersion;

        QJsonObject jsonData = hla->getJsonData();
        jsonData["EpicGames"] = gameJsonData;
        hla->setJsonData(jsonData);
        hla->findMasterGame();
    }
    else {
        QMessageBox::warning(this, tr("Select GTA V Epic Games folder"), "The selected folder is not a valid GTA V folder!");
    }
}

void UserInterface::on_btnOk_clicked()
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
}

UserInterface::~UserInterface()
{
    delete ui;
}
