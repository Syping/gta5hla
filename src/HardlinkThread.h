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

#ifndef HARDLINKTHREAD_H
#define HARDLINKTHREAD_H

// Qt includes
#include <QObject>
#include <QThread>

// gta5hla includes
#include "HardlinkAssistant.h"

class HardlinkThread : public QThread
{
    Q_OBJECT
public:
    explicit HardlinkThread(HardlinkAssistant *hla, const QString &masterGamePath, const QString &slaveGamePath, const QStringList &masterGameFiles, QObject *parent = nullptr);
    bool isOk();

private:
    HardlinkAssistant *hla;
    bool m_ok;
    QString m_masterGamePath;
    QString m_slaveGamePath;
    QStringList m_masterGameFiles;

protected:
    void run();
};

#endif // HARDLINKTHREAD_H
