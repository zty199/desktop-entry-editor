#include "utils.h"

const QString websiteLinkTemplate = "<a href='%1' style='text-decoration: none; color: rgba(0,129,255,0.9);'>%2</a>";

QString Utils::websiteLinkText(const QString &text, const QString &url)
{
    return QString(websiteLinkTemplate).arg(url).arg(text);
}
