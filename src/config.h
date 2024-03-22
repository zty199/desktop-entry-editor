#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class Config
{
public:
    static Config *instance();

    QVariant getValue(const QString &section, const QString &key);
    void setValue(const QString &section, const QString &key, const QVariant &value);

private:
    Config();
    Config(Config &) = delete;
    Config &operator=(const Config &) = delete;
    ~Config();

    void init();

private:
    static Config *m_instance;

    QSettings *m_settings = nullptr;

    QString m_fileName;
};

#endif // CONFIG_H
