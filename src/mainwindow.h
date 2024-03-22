#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <DLabel>
#include <DLineEdit>
#include <DFileChooserEdit>
#include <DSwitchButton>
#include <DComboBox>
#include <DPushButton>
#include <DMessageManager>

#include <XdgDesktopFile>

class MainWindow : public Dtk::Widget::DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DMainWindow *parent = nullptr);
    ~MainWindow() override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    void setDesktopFile(const QString &file);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void initUI();
    void initTitlebar();
    void initDefaultValues();
    void initConnections();

    void checkWorkspace();
    void loadDesktopFile();

private slots:
    void slotIconThemeNameChanged(const QByteArray &iconThemeName);

    void newDesktopFile();
    void openDesktopFile();
    void saveAsDesktopFile();

    void chooseIcon();
    void createOrUpdateDesktopFile();

private:
    Dtk::Widget::DLabel *m_icon = nullptr;
    Dtk::Widget::DLabel *m_iconTip = nullptr;

    Dtk::Widget::DLineEdit *m_nameEdit = nullptr;
    Dtk::Widget::DLineEdit *m_execEdit = nullptr;
    Dtk::Widget::DLineEdit *m_commentEdit = nullptr;
    Dtk::Widget::DLineEdit *m_mimetypeEdit = nullptr;
    Dtk::Widget::DComboBox *m_categories = nullptr;
    Dtk::Widget::DSwitchButton *m_terminal = nullptr;
    Dtk::Widget::DSwitchButton *m_nodisplay = nullptr;

    Dtk::Widget::DFloatingMessage *message = nullptr;
    Dtk::Widget::DPushButton *open = nullptr;

    XdgDesktopFile *m_parser = nullptr;

    QString m_iconFile;
    QString m_desktopFile;
    QString fileName;
    bool isSaveAs = false;
};

#endif // MAINWINDOW_H
