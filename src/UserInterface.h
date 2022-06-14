/*****************************************************************************
* gta5hla GTA V Hardlinking Tool
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

public slots:
    void logUpdated(const QString &text);

private slots:
    void on_btnSocialClub_clicked();
    void on_btnSteam_clicked();
    void on_btnEpicGames_clicked();
    void on_btnLink_clicked();

private:
    HardlinkAssistant *hla;
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
