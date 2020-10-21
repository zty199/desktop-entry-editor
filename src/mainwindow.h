#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include <DMainWindow>
#include <DGuiApplicationHelper>
#include <DLabel>
#include <DLineEdit>
#include <DFileChooserEdit>
#include <DSwitchButton>
#include <DComboBox>
#include <DPushButton>

#include <XdgDesktopFile>

#define WORKSPACE "/.local/share/applications/desktop-entry-editor/"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(DMainWindow *parent = nullptr);
    ~MainWindow();

    void setDesktopFile(QString file);

private:
    QWidget *w = new QWidget;   //  w 是窗口的用户区，应当是所有窗口中控件的父（不包含标题栏及其上边的控件）

    QMenu *m_menu;
    QAction *m_newFile;
    QAction *m_openFile;
    QAction *m_saveFile;
    QAction *m_saveAs;

    DLineEdit *m_nameEdit;
    DLineEdit *m_execEdit;
    DLineEdit *m_commentEdit;
    DLineEdit *m_mimetypeEdit;
    DComboBox *m_categories;
    DSwitchButton *m_terminal;
    DSwitchButton *m_nodisplay;
    DLabel *m_icon;
    DLabel *m_iconTip;

    DPushButton *m_confirm;
    DPushButton *m_exit;

    XdgDesktopFile *m_parser;

    QString m_iconFile;
    QString m_desktopFile;
    QString fileName;

    void initUI();
    void initDefaultValues();
    void initConnections();
    void checkWorkspace();
    void loadDesktopFile();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void newDesktopFile();
    void openDesktopFile();
    void saveAsDesktopFile();
    void chooseIcon();
    void createOrUpdateDesktopFile();
    void exitEditor();

};

#endif // MAINWINDOW_H
