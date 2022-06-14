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

// gta5hla includes
#include "HardlinkThread.h"

HardlinkThread::HardlinkThread(HardlinkAssistant *hla, const QString &masterGamePath, const QString &slaveGamePath, const QStringList &masterGameFiles, QObject *parent) : QThread(parent),
    hla(hla), m_masterGamePath(masterGamePath), m_slaveGamePath(slaveGamePath), m_masterGameFiles(masterGameFiles)
{
}

bool HardlinkThread::isOk()
{
    return m_ok;
}

void HardlinkThread::run()
{
    m_ok = hla->linkGame(m_masterGamePath, m_slaveGamePath, m_masterGameFiles);
}
