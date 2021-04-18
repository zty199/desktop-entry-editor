#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSlider>

#include <DMainWindow>
#include <DLabel>
#include <DLineEdit>
#include <DFileChooserEdit>
#include <DSwitchButton>
#include <DComboBox>
#include <DPushButton>
#include <DMessageManager>

#include <XdgDesktopFile>

#include "settings.h"
#include "about.h"

#define SAVEPATH "/.local/share/applications/desktop-entry-editor/"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(DMainWindow *parent = nullptr);
    ~MainWindow();

    void setDesktopFile(QString file);

private:
    //  w 是窗口的用户区，应当是所有窗口中控件的父（不包含标题栏及其上边的控件）
    QWidget *w;

    //  s 是设置窗口
    Settings *s;

    //  a 是关于窗口
    About *a;

    QMenu *m_menu;
    QAction *m_newFile;
    QAction *m_openFile;
    QAction *m_saveFile;
    QAction *m_saveAs;
    QAction *m_settings;
    QMenu *m_theme;
    QAction *m_lighttheme;
    QAction *m_darktheme;
    QAction *m_systemtheme;
    QAction *m_about;
    QAction *m_exit;

    DLineEdit *m_nameEdit;
    DLineEdit *m_execEdit;
    DLineEdit *m_commentEdit;
    DLineEdit *m_mimetypeEdit;
    DComboBox *m_categories;
    DSwitchButton *m_terminal;
    DSwitchButton *m_nodisplay;
    DLabel *m_icon;
    DLabel *m_iconTip;

    XdgDesktopFile *m_parser;

    DPushButton *open;
    DFloatingMessage *message;

    int opacity = 200;
    bool blur = false;
    QString theme;

    QString m_iconFile;
    QString m_desktopFile;
    QString fileName;
    bool isSaveAs;

    void initThemeMenu();
    void initSettings();
    void initUI();
    void initDefaultValues();
    void initConnections();
    void checkWorkspace();
    void loadDesktopFile();

    void closeEvent(QCloseEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void newDesktopFile();
    void openDesktopFile();
    void saveAsDesktopFile();
    void setOpacity(int value);
    void setBlur(bool flag);
    void setBackgroundColor(QString theme, int value, bool flag);
    void chooseIcon();
    void createOrUpdateDesktopFile();
    void exitEditor();

signals:
    void opacityChanged(int);
    void blurChanged(bool);

};

#endif // MAINWINDOW_H
