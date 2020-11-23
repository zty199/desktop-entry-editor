#ifndef ABOUT_H
#define ABOUT_H

#include <DAboutDialog>

#define ICONNAME "com.gitee.deepin-opensource.desktop-entry-editor"

DWIDGET_USE_NAMESPACE

class About : public DAboutDialog
{
    Q_OBJECT

public:
    explicit About(DAboutDialog *parent = nullptr);
    ~About();

};

#endif // ABOUT_H
