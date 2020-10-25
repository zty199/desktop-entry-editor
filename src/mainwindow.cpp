#include "mainwindow.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDesktopServices>

#include <DWidgetUtil>  //  Dtk::Widget::moveToCenter(&w); 要调用它，就得引用 DWidgetUtil
#include <DTitlebar>
#include <DFileDialog>
#include <DMessageBox>

static const QString KeyEncoding = "Encoding";
static const QString KeyType = "Type";
static const QString KeyName = "Name";
static const QString KeyExec = "Exec";
static const QString KeyIcon = "Icon";
static const QString KeyComment = "Comment";
static const QString KeyCategories = "Categories";
static const QString KeyTerminal = "Terminal";
static const QString KeyNoDisplay = "NoDisplay";
static const QString KeyStartupWMClass = "StartupWMClass";
static const QString KeyMimeType = "MimeType";

MainWindow::MainWindow(DMainWindow *parent)
    : DMainWindow(parent),
      m_menu(new QMenu),
      m_newFile(new QAction(tr("New"))),
      m_openFile(new QAction(tr("Open"))),
      m_saveFile(new QAction(tr("Save"))),
      m_saveAs(new QAction(tr("Save As"))),
      m_nameEdit(new DLineEdit),
      m_execEdit(new DFileChooserEdit),
      m_commentEdit(new DLineEdit),
      m_mimetypeEdit(new DLineEdit),
      m_categories(new DComboBox),
      m_terminal(new DSwitchButton),
      m_nodisplay(new DSwitchButton),
      m_icon(new DLabel),
      m_iconTip(new DLabel),
      m_confirm(new DPushButton),
      m_exit(new DPushButton),
      m_parser(new XdgDesktopFile)
{
    initUI();               //  初始化主窗口
    initDefaultValues();    //  初始化默认值
    initConnections();      //  初始化槽函数
    checkWorkspace();       //  检查工作区
}

MainWindow::~MainWindow()
{
    delete w;
}

void MainWindow::setDesktopFile(QString file)
{
    if(file.endsWith(".desktop"))
    {
        //  获取文件绝对路径
        QDir dir = QDir::currentPath();
        m_desktopFile = dir.absoluteFilePath(file);

        loadDesktopFile();
    }
}

void MainWindow::initUI()
{
    setCentralWidget(w);                              //  将 w 作为窗口的用户部分（整个窗口除了标题栏的部分）
    setFixedSize(600, 450);                           //  固定 MainWindow 窗口大小
    moveToCenter(this);                               //  把窗口移动到屏幕中间

    setWindowIcon(QIcon::fromTheme(ICONNAME));        //  设置窗口图标
    setAttribute(Qt::WA_TranslucentBackground, true); //  设置窗口透明
    titlebar()->setBlurBackground(true);              //  设置标题栏模糊
    titlebar()->setIcon(QIcon::fromTheme(ICONNAME));  //  设置标题栏图标

    //  在标题栏上添加一个菜单 / 菜单项
    m_menu->addAction(m_newFile);
    m_menu->addAction(m_openFile);
    m_menu->addSeparator();
    m_menu->addAction(m_saveFile);
    m_menu->addAction(m_saveAs);
    m_menu->addSeparator();
    titlebar()->setMenu(m_menu);

    //  初始化按钮
    m_confirm->setIcon(QIcon::fromTheme("dialog-apply").pixmap(64, 64));
    m_confirm->setText(tr("Confirm"));
    m_confirm->setFixedSize(100, 50);
    m_exit->setIcon(QIcon::fromTheme("dialog-cancel").pixmap(64, 64));
    m_exit->setText(tr("Exit"));
    m_exit->setFixedSize(100, 50);

    //  页面布局
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(m_icon);
    leftLayout->addSpacing(10);
    leftLayout->addWidget(m_iconTip);

    QGridLayout *rightLayout = new QGridLayout;
    rightLayout->setColumnStretch(1,1); //  设置列伸缩系数为 1
    rightLayout->addWidget(new QLabel(tr("Name")), 0, 0);
    rightLayout->addWidget(m_nameEdit, 0, 1, 1, 2);
    rightLayout->addWidget(new QLabel(tr("Exec")), 1, 0);
    rightLayout->addWidget(m_execEdit, 1, 1, 1, 2);
    rightLayout->addWidget(new QLabel(tr("Comment")), 2, 0);
    rightLayout->addWidget(m_commentEdit, 2, 1, 1, 2);
    rightLayout->addWidget(new QLabel(tr("MimeType")), 3, 0);
    rightLayout->addWidget(m_mimetypeEdit, 3, 1, 1, 2);
    rightLayout->addWidget(new QLabel(tr("Categories")), 4, 0);
    rightLayout->addWidget(m_categories, 4, 1, 1, 2);
    rightLayout->addWidget(new QLabel(tr("Run in Terminal")), 5, 0, 1, 2);
    rightLayout->addWidget(m_terminal, 5, 2, 1, 1);
    rightLayout->addWidget(new QLabel(tr("Not Display in Launcher")), 6, 0, 1, 2);
    rightLayout->addWidget(m_nodisplay, 6, 2, 1, 1);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addSpacing(30);
    topLayout->setAlignment(Qt::AlignCenter);
    topLayout->addLayout(leftLayout);
    topLayout->addSpacing(30);
    topLayout->addLayout(rightLayout);
    topLayout->addSpacing(30);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->setAlignment(Qt::AlignCenter);
    bottomLayout->addWidget(m_confirm);
    bottomLayout->addSpacing(80);
    bottomLayout->addWidget(m_exit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(topLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(bottomLayout);

    w->setLayout(mainLayout);
}

void MainWindow::initDefaultValues()
{
    setAcceptDrops(true);                               //  允许窗口接受拖放文件
    titlebar()->setTitle(tr("Desktop Entry Editor"));   //  设置标题栏标题

    m_iconFile = nullptr;
    m_desktopFile = nullptr;
    fileName = nullptr;
    isSaveAs = false;

    m_icon->installEventFilter(this);                   //  响应点击图标事件
    m_icon->setPixmap(QIcon::fromTheme("application-x-executable").pixmap(128, 128));
    m_iconTip->installEventFilter(this);
    m_iconTip->setStyleSheet("color:SteelBlue");
    m_iconTip->setText(tr("Click to choose icon"));
    m_iconTip->setAlignment(Qt::AlignCenter);

    m_nameEdit->clear();
    m_nameEdit->setPlaceholderText(tr("Name to display"));
    m_commentEdit->clear();
    m_commentEdit->setPlaceholderText(tr("Comment of this .desktop"));
    m_execEdit->clear();
    m_execEdit->setPlaceholderText(tr("Choose a binary"));
    m_mimetypeEdit->clear();
    m_mimetypeEdit->setPlaceholderText(tr("Supported MIME type(s)"));

    m_terminal->setChecked(false);
    m_nodisplay->setChecked(false);

    m_categories->clear();
    QMap<QString, QString> Categories;
    Categories.insert(tr("- Please Select -"), "");
    Categories.insert(tr("Network"), "Network");
    Categories.insert(tr("Chat"), "Chat");
    Categories.insert(tr("Music"), "Music");
    Categories.insert(tr("Video"), "Video");
    Categories.insert(tr("Graphics"), "Graphics");
    Categories.insert(tr("Game"), "Game");
    Categories.insert(tr("Office"), "Office");
    Categories.insert(tr("Translation"), "Translation");
    Categories.insert(tr("Development"), "Development");
    Categories.insert(tr("System"), "System");
    Categories.insert(tr("Others"), "Others");
    foreach(const QString &str, Categories.keys())
        m_categories->addItem(str, Categories.value(str));
    m_categories->setCurrentIndex(0);
}

void MainWindow::initConnections()
{
    connect(m_newFile, &QAction::triggered, this, [ = ] () { newDesktopFile(); });
    connect(m_openFile, &QAction::triggered, this, [ = ] () { openDesktopFile(); });
    connect(m_saveFile, &QAction::triggered, this, [ = ] () { createOrUpdateDesktopFile(); });
    connect(m_saveAs, &QAction::triggered, this, [ = ] () { saveAsDesktopFile(); });
    connect(m_confirm, SIGNAL(clicked()), this, SLOT(createOrUpdateDesktopFile()));
    connect(m_exit, SIGNAL(clicked()), this, SLOT(exitEditor()));
}

void MainWindow::checkWorkspace()
{
    QDir workspace;
    if(!workspace.exists(QDir::homePath() + WORKSPACE))
    {
        workspace.mkdir(QDir::homePath() + WORKSPACE);
    }
}

void MainWindow::loadDesktopFile()
{
    if(!m_desktopFile.isEmpty())
    {
        QFile file(m_desktopFile);
        //  判断文件是否存在，以及文件是否可读取
        if(file.exists() && file.permissions().testFlag(QFile::ReadOwner))
        {
            fileName = QFileInfo(file).fileName();              //  获取文件名
            titlebar()->setTitle(fileName);                     //  显示当前文件名
            m_parser->load(QFileInfo(file).absoluteFilePath()); //  载入文件（会自动清空原有键值）
            m_desktopFile = QFileInfo(file).absoluteFilePath(); //  更新文件绝对路径
        }
        else
        {
            return;
        }

        isSaveAs = false;

        //  读取文件信息
        m_nameEdit->setText(m_parser->value(KeyName).toString());
        m_commentEdit->setText(m_parser->value(KeyComment).toString());
        m_execEdit->setText(m_parser->value(KeyExec).toString());
        m_mimetypeEdit->setText(m_parser->value(KeyMimeType).toString());

        m_categories->clear();
        QMap<QString, QString> Categories;
        if(m_parser->value(KeyCategories).toString().isEmpty())
        {
            Categories.insert(tr("- Please Select -"), "");
        }
        else
        {
            Categories.insert(m_parser->value(KeyCategories).toString(), m_parser->value(KeyCategories).toString());
        }
        Categories.insert(tr("Network"), "Network");
        Categories.insert(tr("Chat"), "Chat");
        Categories.insert(tr("Music"), "Music");
        Categories.insert(tr("Video"), "Video");
        Categories.insert(tr("Graphics"), "Graphics");
        Categories.insert(tr("Game"), "Game");
        Categories.insert(tr("Office"), "Office");
        Categories.insert(tr("Translation"), "Translation");
        Categories.insert(tr("Development"), "Development");
        Categories.insert(tr("System"), "System");
        Categories.insert(tr("Others"), "Others");
        foreach(const QString &str, Categories.keys())
            m_categories->addItem(str, Categories.value(str));
        m_categories->setCurrentIndex(0);

        m_terminal->setChecked(m_parser->value(KeyTerminal).toBool());
        m_nodisplay->setChecked(m_parser->value(KeyNoDisplay).toBool());

        m_iconFile = m_parser->value(KeyIcon).toString();
        if(QFile(m_iconFile).exists())
        {
            m_icon->setPixmap(QIcon(m_iconFile).pixmap(128, 128));
        }
        else
        {
            m_icon->setPixmap(QIcon::fromTheme(m_iconFile, QIcon::fromTheme("application-x-executable")).pixmap(128, 128));
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString dropFile = event->mimeData()->urls().at(0).toLocalFile();
    if(QFileInfo(dropFile).suffix() == "desktop")
    {
        m_desktopFile = dropFile;
        loadDesktopFile();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if((qobject_cast<QLabel*>(obj) == m_icon || qobject_cast<QLabel*>(obj) == m_iconTip) && event->type() == QEvent::MouseButtonRelease)
    {
        chooseIcon();
        return true;
    }
    else
    {
        return DMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::newDesktopFile()
{
    initDefaultValues();
}

void MainWindow::openDesktopFile()
{
    QString openFile = QFileDialog::getOpenFileName(this, tr("Select file"), QDir::homePath(), tr("All Files (*)"));
    if(QFileInfo(openFile).suffix() == "desktop")
    {
        m_desktopFile = openFile;
        loadDesktopFile();
    }
}

void MainWindow::saveAsDesktopFile()
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath() + WORKSPACE, tr("Desktop Entry Files (*.desktop)"));
    if(!saveFile.isEmpty())
    {
        //  判断上层目录是否可写入
        if(QFileInfo(QFileInfo(saveFile).absolutePath()).permissions().testFlag(QFile::WriteUser))
        {
            m_desktopFile = saveFile;
            if(!saveFile.endsWith(".desktop"))
            {
                m_desktopFile += ".desktop";
                isSaveAs = true;    //  另存为文件时文件不存在，所以无法写入，需要判断
            }
            createOrUpdateDesktopFile();
        }
        else
        {
            saveAsDesktopFile();
            return;
        }
    }
}

void MainWindow::chooseIcon()
{
    QString iconFile = QFileDialog::getOpenFileName(this, tr("Select icon"), QDir::homePath(), tr("Image Files (*.png *.jpg *.svg)"));
    if(!iconFile.isEmpty())
    {
        m_iconFile = iconFile;
        m_icon->setPixmap(QIcon(m_iconFile).pixmap(128, 128));
    }
}

void MainWindow::createOrUpdateDesktopFile()
{
    //  判断文件路径是否为空，原文件是否可写入，是否为另存为操作
    if(m_desktopFile.isEmpty() || (!QFile::permissions(m_desktopFile).testFlag(QFile::WriteOwner) && !isSaveAs))
    {
        saveAsDesktopFile();
        return;
    }

    m_parser->setValue(KeyEncoding, "UTF-8");
    m_parser->setValue(KeyType, "Application");
    m_parser->setValue(KeyName, m_nameEdit->text());
    m_parser->setValue(KeyExec, m_execEdit->text());
    m_parser->setValue(KeyIcon, m_iconFile);

    if(!m_commentEdit->text().isEmpty())
    {
        m_parser->setValue(KeyComment, m_commentEdit->text());
    }
    else
    {
        m_parser->removeEntry(KeyComment);
    }

    if(m_categories->currentIndex() == 0 && !m_categories->currentData().toString().isEmpty())
    {
        m_parser->setValue(KeyCategories, m_categories->currentData());
    }
    else if(m_categories->currentIndex() != 0)
    {
        m_parser->setValue(KeyCategories, m_categories->currentData().toString() + ";");
    }
    else
    {
        m_parser->removeEntry(KeyCategories);
    }

    if(m_terminal->isChecked())
    {
        m_parser->setValue(KeyTerminal, "True");
    }
    else
    {
        m_parser->setValue(KeyTerminal, "False");
    }

    if(m_nodisplay->isChecked())
    {
        m_parser->setValue(KeyNoDisplay, "True");
    }
    else
    {
        m_parser->removeEntry(KeyNoDisplay);
    }

    if(m_parser->value(KeyStartupWMClass).toString().isEmpty() && m_nameEdit->text().isEmpty())
    {
        m_parser->removeEntry(KeyStartupWMClass);
    }
    else
    {
        m_parser->setValue(KeyStartupWMClass, m_nameEdit->text());
    }

    m_parser->setValue(KeyMimeType, m_mimetypeEdit->text());

    m_parser->save(m_desktopFile);

    fileName = QFileInfo(m_desktopFile).fileName();
    titlebar()->setTitle(fileName); //  标题栏显示当前文件名

    DMessageBox::StandardButton result = DMessageBox::information(nullptr, tr("Tip"), tr("Desktop Entry saved!"), DMessageBox::Ok | DMessageBox::Open);
    switch (result)
    {
        case DMessageBox::Ok:
            return;
        case DMessageBox::Open:
            //  该方法可以直接打开文件或文件夹，但是文件打开受默认打开的方式影响，而打开上层文件夹又无法选中文件位置，故舍弃
            //  QDesktopServices::openUrl(QFileInfo(m_desktopFile).absolutePath());

            //  依赖文件管理器显示文件所在文件位置，方便使用其他编辑器修改文件
            process = new QProcess;
            process->start("dde-file-manager --show-item " + m_desktopFile);    //  非阻塞调用外部程序，不需要等待结束
            break;
        default:
            return;
    }
}

void MainWindow::exitEditor()
{
    this->close();
}
