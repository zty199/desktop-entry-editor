#include "about.h"

About::About(DAboutDialog *parent) :
    DAboutDialog(parent)
{
    setProductIcon(QIcon::fromTheme(ICONNAME)); //  设置产品图标
    setProductName(tr("Desktop Entry Editor")); //  设置产品名称

    //  设置应用版本
    QString version = tr("Version: ");
    version += "1.2";
    setVersion(version);

    //  setCompanyLogo();                           //  设置公司图标
    //  setWebsiteName("");                         //  设置官网名称
    //  setWebsiteLink("");                         //  设置官网链接

    //  设置应用描述
    setDescription(
                QObject::tr(
                    "<a href='https://github.com/zty199/desktop-entry-editor'>https://github.com/zty199/desktop-entry-editor</a><br/><br/>"
                    "A simple GUI tool to create and edit desktop files<br/>Desinged for Deepin<br/><br/>"
                    "Powered by Deepin Community<br/>"
                    "<a href='https://www.deepinos.org/'><span style='font-size:14pt;'>Spark developers</span></a><br/><br/>"
                    "Published under GPL V3"
                )
    );
}

About::~About()
{
}
