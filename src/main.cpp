#include "mainwindow.h"

#include <DApplication>

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);

    a.loadTranslator();                                                                 //  加载翻译
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setOrganizationName("spark-union");                                               //  设置组织名称
    a.setApplicationVersion(DApplication::buildVersion("1.0"));                         //  设置程序版本
    a.setProductIcon(QIcon::fromTheme("application-x-desktop"));                        //  设置程序Logo
    a.setProductName("Desktop Entry Editor");                                           //  设置项目名称
    a.setApplicationDescription("A simple Desktop Entry editor\nDeveloped using DTK");  //  设置程序介绍
    a.setApplicationName("Desktop Entry Editor");                                       //  只有在这儿修改窗口标题才有效

    MainWindow w;
    if(argc >= 2)
    {
        w.setDesktopFile(argv[1]);
    }
    w.show();

    return a.exec();
}
