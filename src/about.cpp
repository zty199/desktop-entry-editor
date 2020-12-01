#include "about.h"

About::About(DAboutDialog *parent) :
    DAboutDialog(parent)
{
    setWindowIcon(QIcon::fromTheme(ICONNAME));          //  设置窗口图标
    setProductIcon(QIcon::fromTheme(ICONNAME));         //  设置产品图标
    setProductName(tr("Desktop Entry Editor"));         //  设置产品名称
    setVersion(QString(tr("Version: %1")).arg("1.4"));  //  设置应用版本

    //  setCompanyLogo();                               //  设置公司图标
    //  setWebsiteName("");                             //  设置官网名称
    //  setWebsiteLink("");                             //  设置官网链接

    //  设置应用描述
    setDescription(
                QString(
                    "<a href='https://github.com/zty199/desktop-entry-editor'>https://github.com/zty199/desktop-entry-editor</a><br/><br/>"
                    "%1<br/>%2<br/><br/>"
                    "%3<br/>"
                    "<a href='https://www.deepinos.org/'><span style='font-size:14pt;'>%4</span></a><br/><br/>"
                    )
                .arg(tr("A simple GUI tool to create and edit desktop files"))
                .arg(tr("Desinged for Deepin"))
                .arg(tr("Powered by Deepin Community"))
                .arg(tr("Spark Developers"))
                );

    setLicense(tr("Published under GPL V3"));
}

About::~About()
{
}
