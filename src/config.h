#ifndef CONFIG_H
#define CONFIG_H

#include <QDir>
#include <QSettings>

#define CONFIGPATH "/.config/desktop-entry-editor/"

class Config
{
public:
    Config();
    ~Config();

    QVariant getSettings(QString section, QString key);
    void setSettings(QString section, QString key, QVariant value);

private:
    QString m_fileName = QDir::homePath() + CONFIGPATH + "config.ini";
    QSettings *m_settings;

};

#endif // CONFIG_H
