#ifndef ABOUT_H
#define ABOUT_H

#include <DAboutDialog>

#define ICONNAME "com.gitee.deepin-opensource.desktop-entry-editor"

DWIDGET_USE_NAMESPACE

namespace Ui {
class About;
}

class About : public DAboutDialog
{
    Q_OBJECT

public:
    explicit About(DAboutDialog *parent = nullptr);
    ~About();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
