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

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

// Qt includes
#include <QDialog>

// gta5hla includes
#include "HardlinkAssistant.h"

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProgressDialog(HardlinkAssistant *hla, QWidget *parent = nullptr);
    void setLinkLabelText(const QString &text);
    ~ProgressDialog();

public slots:
    void linkProgressCurrentUpdated(int current);
    void linkProgressMaxUpdated(int max);
    void appendLog(const QString &text);

private:
    HardlinkAssistant *hla;
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
