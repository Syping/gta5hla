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
#include <QApplication>
#include <QJsonDocument>

// gta5hla includes
#include "HardlinkAssistant.h"
#include "UserInterface.h"

int main(int argc, char *argv[])
{
#if QT_VERSION >= 0x050600
#if QT_VERSION < 0x060000
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif
#endif
    QApplication a(argc, argv);
    a.setApplicationName("gta5hla");
    a.setApplicationVersion("0.1.0");

#ifdef Q_OS_WIN
    a.setFont(QApplication::font("QMenu"));
#endif

    HardlinkAssistant hla;

#ifdef GTA5HLA_DEBUG
    QJsonDocument jsonDocument;
    jsonDocument.setObject(hla.getJsonData());
    qDebug() << jsonDocument.toJson(QJsonDocument::Compact);
#endif

#ifdef GTA5HLA_GUI
    UserInterface appUi(&hla);
    appUi.show();

    return a.exec();
#else
    return 0;
#endif
}
