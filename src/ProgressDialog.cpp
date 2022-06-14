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

// gta5hla includes
#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"

ProgressDialog::ProgressDialog(HardlinkAssistant *hla, QWidget *parent) : QDialog(parent),
    hla(hla), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    QObject::connect(hla, &HardlinkAssistant::linkProgressCurrentUpdated, this, &ProgressDialog::linkProgressCurrentUpdated);
    QObject::connect(hla, &HardlinkAssistant::linkProgressMaxUpdated, this, &ProgressDialog::linkProgressMaxUpdated);
    QObject::connect(hla, &HardlinkAssistant::logUpdated, this, &ProgressDialog::appendLog);
}

void ProgressDialog::setLinkLabelText(const QString &text)
{
    ui->labLink->setText(text);
}

void ProgressDialog::linkProgressCurrentUpdated(int current)
{
    ui->pbProgress->setValue(current);
}

void ProgressDialog::linkProgressMaxUpdated(int max)
{
    ui->pbProgress->setMaximum(max);
}

void ProgressDialog::appendLog(const QString &text)
{
    ui->txtLog->appendPlainText(text);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
