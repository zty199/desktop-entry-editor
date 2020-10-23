#include "mainwindow.h"

#include <DApplication>

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);

    a.loadTranslator();                                         //  加载翻译
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    a.setOrganizationName("spark-union");                       //  设置组织名称
    a.setProductName(QObject::tr("Desktop Entry Editor"));      //  设置项目名称
    a.setProductIcon(QIcon::fromTheme(ICONNAME));               //  设置程序 Logo
    a.setApplicationName(QObject::tr("Desktop Entry Editor"));  //  设置程序名称
    a.setApplicationVersion(DApplication::buildVersion("1.2")); //  设置程序版本

    //  设置程序介绍
    a.setApplicationDescription(
                QObject::tr(
                    "<a href='https://github.com/zty199/desktop-entry-editor'>https://github.com/zty199/desktop-entry-editor</a><br/><br/>"
                    "A simple GUI tool to create and edit desktop files for Deepin<br/>Desinged using DTK<br/><br/>"
                    "<span style='font-size:10pt;font-weight:60;'>Powered by Deepin Community</span><br/>"
                    "<a href='https://www.deepinos.org/'><span style='font-size:12pt;'>Spark developers</span></a><br/><br/>"
                    "Published under GPL V3"
                    )
                );

    MainWindow w;
    if(argc >= 2)
    {
        w.setDesktopFile(argv[1]);
    }
    w.show();

    return a.exec();
}
