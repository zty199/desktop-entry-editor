#include "config.h"

#include <QStandardPaths>
#include <QDir>

Config *Config::m_instance = nullptr;

Config *Config::instance()
{
    if (m_instance == nullptr) {
        m_instance = new Config;
    }

    return m_instance;
}

QVariant Config::getValue(const QString &section, const QString &key)
{
    return m_settings->value(QString("/%1/%2").arg(section).arg(key));
}

void Config::setValue(const QString &section, const QString &key, const QVariant &value)
{
    m_settings->setValue(QString("/%1/%2").arg(section).arg(key), value);
    m_settings->sync();
}

Config::Config()
    : m_fileName(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/config.ini")
{
    init();

    m_settings = new QSettings(m_fileName, QSettings::IniFormat);
}

Config::~Config()
{
    m_settings->sync();
    delete m_settings;
}

void Config::init()
{
    QDir dir;
    dir.mkpath(QFileInfo(m_fileName).absolutePath());
}
