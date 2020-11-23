#include "settings.h"
#include "ui_settings.h"

Settings::Settings(DBlurEffectWidget *parent) :
    DBlurEffectWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    initUI();

    connect(ui->slider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] { setBackgroundColor(); });
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [ = ] { setOpacity(ui->slider->value()); });
}

Settings::~Settings()
{
    delete ui;
}

QSlider* Settings::getSlider()
{
    return ui->slider;
}

DSwitchButton* Settings::getSwitchButton()
{
    return ui->switchbutton;
}

void Settings::initUI()
{
    setFixedSize(400, 300);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    setMaskAlpha(200);

    ui->titlebar->setBackgroundTransparent(true);
    ui->titlebar->setMenuDisabled(true);
    ui->titlebar->setMenuVisible(false);
}

void Settings::setBackgroundColor()
{
    if(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType)
    {
        setMaskColor(QColor(240, 240, 240));
    }
    else
    {
        setMaskColor(QColor(40, 40, 40));
    }
}

void Settings::setOpacity(int value)
{
    ui->label->setText(QString::number(value / 255.0, 'f', 2));
}
