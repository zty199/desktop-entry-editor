#include "config.h"

Config::Config()
{
    m_settings = new QSettings(m_fileName, QSettings::IniFormat);
}

Config::~Config()
{
    delete m_settings;
}

QVariant Config::getSettings(QString section, QString key)
{
    return m_settings->value(QString("/%1/%2").arg(section).arg(key));
}

void Config::setSettings(QString section, QString key, QVariant value)
{
    m_settings->setValue(QString("/%1/%2").arg(section).arg(key), value);
}
