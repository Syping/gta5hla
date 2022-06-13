#ifndef HARDLINKASSISTANT_H
#define HARDLINKASSISTANT_H

// Qt includes
#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class HardlinkAssistant : public QObject
{
public:
    explicit HardlinkAssistant(QObject *parent = nullptr);
    void findPaths();
    void findMasterGame();
    const QString& getMasterGame();
    const QJsonObject& getJsonData();
    static const QString getGameVersion(const QString &dirPath);
    static const QString getExecutableVersion(const QString &filePath);
    void setMasterGame(const QString &masterGame);
    void setJsonData(QJsonObject jsonData);

private:
    QString m_masterGame;
    QJsonObject m_jsonData;
};

#endif // HARDLINKASSISTANT_H
