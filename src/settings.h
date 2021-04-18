#ifndef SETTINGS_H
#define SETTINGS_H

#include <DBlurEffectWidget>
#include <DMainWindow>

DWIDGET_USE_NAMESPACE

namespace Ui {
class Settings;
}

class Settings : public DBlurEffectWidget
{
    Q_OBJECT

public:
    explicit Settings(DMainWindow *m, DBlurEffectWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    DMainWindow *m;

    void initUI();

private slots:
    void setBackgroundColor();
    void setOpacity(int value);
    void setBlur(bool checked);
    void on_opacityChanged(int value);
    void on_blurChanged(bool flag);

signals:
    void opacityChanged(int);
    void blurChanged(bool);

};

#endif // SETTINGS_H
