#ifndef UTILS_H
#define UTILS_H

#include <QObject>

extern const QString websiteLinkTemplate;

class Utils
{
public:
    static QString websiteLinkText(const QString &text, const QString &url = QString());
};

#endif // UTILS_H
