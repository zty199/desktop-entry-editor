#include "settings.h"
#include "ui_settings.h"

#include <DWidgetUtil>

Settings::Settings(DMainWindow *m, DBlurEffectWidget *parent)
    : DBlurEffectWidget(parent)
    , ui(new Ui::Settings)
    , m(m)
{
    ui->setupUi(this);

    initUI();

    connect(ui->slider, &QSlider::valueChanged, this, &Settings::setOpacity);
    connect(ui->switchbutton, &DSwitchButton::toggled, this, &Settings::setBlur);
    connect(m, SIGNAL(opacityChanged(int)), this, SLOT(on_opacityChanged(int)));
    connect(m, SIGNAL(blurChanged(bool)), this, SLOT(on_blurChanged(bool)));
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [=]{setBackgroundColor();});
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [=]{emit ui->slider->valueChanged(ui->slider->value());});
}

Settings::~Settings()
{
    delete ui;
}

void Settings::initUI()
{
    setFixedSize(400, 300);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
    moveToCenter(this);

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

    setMaskAlpha(static_cast<quint8>(value));
    emit opacityChanged(value);
}

void Settings::setBlur(bool checked)
{
    Q_UNUSED(checked)

    emit blurChanged(ui->switchbutton->isChecked());
}

void Settings::on_opacityChanged(int value)
{
    ui->slider->setValue(value);
}

void Settings::on_blurChanged(bool flag)
{
    ui->switchbutton->setChecked(flag);
}
