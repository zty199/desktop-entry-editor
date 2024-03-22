#include "application.h"
#include "utils.h"

#include <DLog>
#include <DAboutDialog>
#if DTK_VERSION >= DTK_VERSION_CHECK(5, 6, 4, 0)
#include <DFeatureDisplayDialog>
#endif

#include <QIcon>

Application::Application(int &argc, char **argv)
    : Dtk::Widget::DApplication(argc, argv)
{
    setAttribute(Qt::AA_UseHighDpiPixmaps);

    /**
     * NOTE: load qm files after applicationName is set
     */
    loadTranslator();

    setProductIcon(QIcon::fromTheme("desktop-entry-editor"));
    setApplicationDisplayName(QObject::tr("Desktop Entry Editor"));
    setProductName(applicationDisplayName());
    setApplicationVersion(APP_VERSION);
    setApplicationHomePage("https://github.com/zty199/desktop-entry-editor");
    setApplicationDescription(QObject::tr("A simple GUI tool to create and edit desktop files, designed for Deepin."));
    setApplicationLicense(QString(" %1 ").arg(Utils::websiteLinkText("GPLv3", "https://github.com/zty199/desktop-entry-editor?tab=GPL-3.0-1-ov-file#readme")));

    /**
     * NOTE: acknowledgement is deprecated
     * use DAboutDialog::setWebsiteName and DAboutDialog::setWebsiteLink instead
     */
    //    setApplicationAcknowledgementPage("https://github.com/zty199/desktop-entry-editor");
    //    setApplicationAcknowledgementVisible(true);

    /**
     * BUG: wayland 显示协议下，若不设置 desktopFileName，
     * 默认使用 OrganizationDomain.ApplicationName 作为 appid 匹配 desktop 文件
     */
    setDesktopFileName("desktop-entry-editor");
}

void Application::registerLogAppender()
{
    Dtk::Core::DLogManager::registerConsoleAppender();
    /**
     * NOTE: log file generated in ~/.cache/${OrgName}/${AppName}
     */
    Dtk::Core::DLogManager::registerFileAppender();
}

/*!
  \brief 用于处理菜单中关于按钮的点击

  Triggered when user clicked the about menu item of this window's titlebar,
  default action is to show the about dialog of this window(if there is one),
  override this method if you want to change the default action.

  该函数在用户点击窗口标题栏的关于按钮时触发，默认实现为打开程序关于对话框，
  子类可以重现实现此函数以覆盖其默认行为。
 */
void Application::handleAboutAction()
{
    if (aboutDialog()) {
        return DApplication::handleAboutAction();
    }

    initAboutDialog();
#if DTK_VERSION >= DTK_VERSION_CHECK(5, 6, 4, 0)
    initFeatureDisplayDialog();
#endif
    DApplication::handleAboutAction();
}

void Application::initAboutDialog()
{
    if (aboutDialog()) {
        aboutDialog()->deleteLater();
        setAboutDialog(nullptr);
    }

    Dtk::Widget::DAboutDialog *dialog = new Dtk::Widget::DAboutDialog(activeWindow());
    dialog->setProductIcon(productIcon());
    dialog->setProductName(productName());
    dialog->setVersion(translate("DAboutDialog", "Version: %1").arg(applicationVersion()));
    if (runtimeDtkVersion() >= DTK_VERSION_CHECK(5, 6, 4, 0)) {
        dialog->setVersion(applicationVersion());
    }
    dialog->setWebsiteName("github.com/zty199/desktop-entry-editor");
    dialog->setWebsiteLink("https://github.com/zty199/desktop-entry-editor");
    dialog->setDescription(applicationDescription());
    if (!applicationLicense().isEmpty()) {
        dialog->setLicense(translate("DAboutDialog", "%1 is released under %2").arg(productName()).arg(applicationLicense()));
    }

    //    if (!applicationAcknowledgementPage().isEmpty()) {
    //        dialog->setAcknowledgementLink(applicationAcknowledgementPage());
    //    }
    //    dialog->setAcknowledgementVisible(applicationAcknowledgementVisible());

    /**
     * 目前的关于对话框是非模态的，这里的处理是防止关于对话框可以打开多个的情况
     * 不能使用 aboutToClose 信号，应用能够打开多个的情况下，打开关于后直接关闭程序
     * 此时 aboutToColose 信号不会触发，再次打开程序并打开关于会出现访问野指针，程序崩溃的情况
     */
    setAboutDialog(dialog);
    connect(aboutDialog(), &Dtk::Widget::DAboutDialog::destroyed, this, [=]() {
        setAboutDialog(nullptr);
    });
}

#if DTK_VERSION >= DTK_VERSION_CHECK(5, 6, 4, 0)
void Application::initFeatureDisplayDialog()
{
    if (featureDisplayDialog()) {
        featureDisplayDialog()->deleteLater();
        setFeatureDisplayDialog(nullptr);
    }

    Dtk::Widget::DFeatureDisplayDialog *dialog = new Dtk::Widget::DFeatureDisplayDialog(activeWindow());
    setFeatureDisplayDialog(dialog);
    connect(featureDisplayDialog(), &Dtk::Widget::DAboutDialog::destroyed, this, [=] {
        setFeatureDisplayDialog(nullptr);
    });

    connect(aboutDialog(), &Dtk::Widget::DAboutDialog::featureActivated, this, [=]() {
        featureDisplayDialog()->show();
    });
}
#endif
