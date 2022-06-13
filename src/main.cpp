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
    a.setApplicationVersion("0.1");

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
