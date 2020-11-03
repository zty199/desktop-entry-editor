#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSlider>

#include <DBlurEffectWidget>
#include <DSwitchButton>

DWIDGET_USE_NAMESPACE

namespace Ui {
class Settings;
}

class Settings : public DBlurEffectWidget
{
    Q_OBJECT

public:
    explicit Settings(DBlurEffectWidget *parent = nullptr);
    ~Settings();

    QSlider* getSlider();
    DSwitchButton* getSwitchButton();

private:
    Ui::Settings *ui;

    void initUI();

private slots:
    void setBackgroundColor();
    void setOpacity(int value);

};

#endif // SETTINGS_H
