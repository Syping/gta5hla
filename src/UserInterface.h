#ifndef USERINTERFACE_H
#define USERINTERFACE_H

// Qt includes
#include <QDialog>

// gta5hla includes
#include "HardlinkAssistant.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QDialog
{
    Q_OBJECT
public:
    explicit UserInterface(HardlinkAssistant *hla, QWidget *parent = nullptr);
    ~UserInterface();

private slots:
    void on_btnSocialClub_clicked();
    void on_btnSteam_clicked();
    void on_btnEpicGames_clicked();
    void on_btnOk_clicked();

private:
    HardlinkAssistant *hla;
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
