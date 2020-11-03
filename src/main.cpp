#include "mainwindow.h"

#include <DApplication>

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);

    a.loadTranslator();                                         //  加载翻译
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);

    a.setApplicationName(QObject::tr("Desktop Entry Editor"));  //  设置应用名称

    /*
    a.setOrganizationName("spark-union");                       //  设置组织名称
    a.setProductName(QObject::tr("Desktop Entry Editor"));      //  设置产品名称
    a.setProductIcon(QIcon::fromTheme(ICONNAME));               //  设置产品图标
    a.setApplicationName(QObject::tr("Desktop Entry Editor"));  //  设置应用名称
    a.setApplicationVersion(DApplication::buildVersion("1.2")); //  设置应用版本

    //  设置应用介绍
    a.setApplicationDescription(
                QObject::tr(
                    "<a href='https://github.com/zty199/desktop-entry-editor'>https://github.com/zty199/desktop-entry-editor</a><br/><br/>"
                    "A simple GUI tool to create and edit desktop files<br/>Desinged for Deepin<br/><br/>"
                    "Powered by Deepin Community<br/>"
                    "<a href='https://www.deepinos.org/'><span style='font-size:14pt;'>Spark developers</span></a><br/><br/>"
                    "Published under GPL V3"
                )
    );
    */

    MainWindow w;
    if(argc >= 2)
    {
        w.setDesktopFile(argv[1]);
    }
    w.show();

    return a.exec();
}
