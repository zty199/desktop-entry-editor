#include "mainwindow.h"
#include "config.h"

#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

#include <DWidgetUtil>  //  Dtk::Widget::moveToCenter(&w); 要调用它，就得引用 DWidgetUtil
#include <DTitlebar>
#include <DHorizontalLine>
#include <DDesktopServices>

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
    : DMainWindow(parent)
    , w(new QWidget)
    , s(new Settings(this))
    , a(new About)
    , m_menu(new QMenu)
    , m_newFile(new QAction(tr("New")))
    , m_openFile(new QAction(tr("Open")))
    , m_saveFile(new QAction(tr("Save")))
    , m_saveAs(new QAction(tr("Save As")))
    , m_settings(new QAction(tr("Settings")))
    , m_theme(new QMenu(tr("Theme")))
    , m_lighttheme(new QAction(tr("Light theme")))
    , m_darktheme(new QAction(tr("Dark theme")))
    , m_systemtheme(new QAction(tr("System theme")))
    , m_about(new QAction(tr("About")))
    , m_exit(new QAction(tr("Exit")))
    , m_nameEdit(new DLineEdit)
    , m_execEdit(new DFileChooserEdit)
    , m_commentEdit(new DLineEdit)
    , m_mimetypeEdit(new DLineEdit)
    , m_categories(new DComboBox)
    , m_terminal(new DSwitchButton)
    , m_nodisplay(new DSwitchButton)
    , m_icon(new DLabel)
    , m_iconTip(new DLabel)
    , open(new DPushButton(tr("Open")))
    , message(new DFloatingMessage(DFloatingMessage::ResidentType))
{
    checkWorkspace();       //  检查工作区
    initThemeMenu();        //  初始化主题子菜单
    initSettings();         //  初始化设置
    initUI();               //  初始化主窗口
    initDefaultValues();    //  初始化默认值
    initConnections();      //  初始化槽函数
}

MainWindow::~MainWindow()
{
    delete s;
    delete a;
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

void MainWindow::checkWorkspace()
{
    QDir workspace;
    if(!workspace.exists(QDir::homePath() + SAVEPATH))
    {
        workspace.mkdir(QDir::homePath() + SAVEPATH);
    }
    if(!workspace.exists(QDir::homePath() + CONFIGPATH))
    {
        workspace.mkdir(QDir::homePath() + CONFIGPATH);
    }
}

void MainWindow::initThemeMenu()
{
    m_lighttheme->setCheckable(true);
    m_darktheme->setCheckable(true);
    m_systemtheme->setCheckable(true);
    m_lighttheme->setChecked(false);
    m_darktheme->setChecked(false);
    m_systemtheme->setChecked(false);
}

void MainWindow::initSettings()
{
    QFile config(QDir::homePath() + CONFIGPATH + "config.ini");
    if(config.exists())
    {
        opacity = Config().getSettings("settings", "opacity").toInt();
        if(opacity < 0 || opacity > 255)
        {
            opacity = 200;
        }
        // s->getSlider()->setValue(opacity);
        emit opacityChanged(opacity);

        blur = Config().getSettings("settings", "blur").toBool();
        // s->getSwitchButton()->setChecked(blur);
        emit blurChanged(blur);

        theme = Config().getSettings("settings", "theme").toString();

        setBackgroundColor(theme, opacity, blur);
    }
}

void MainWindow::initUI()
{
    setCentralWidget(w);                                //  将 w 作为窗口的用户部分（整个窗口除了标题栏的部分）
    setFixedSize(600, 450);                             //  固定 MainWindow 窗口大小
    moveToCenter(this);                                 //  把窗口移动到屏幕中间

    setWindowIcon(QIcon::fromTheme(ICONNAME));          //  设置窗口图标
    setAttribute(Qt::WA_TranslucentBackground, true);   //  设置窗口透明
    titlebar()->setIcon(QIcon::fromTheme(ICONNAME));    //  设置标题栏图标
    //  titlebar()->setBackgroundTransparent(true);         //  设置标题栏透明
    //  setTitlebarShadowEnabled(false);                    //  关闭标题栏阴影

    w->setAutoFillBackground(true);                     //  背景自动填充
    //  titlebar()->setAutoFillBackground(true);

    titlebar()->setQuitMenuDisabled(true);              //  关闭原生菜单

    /* 在标题栏上添加一个菜单 / 菜单项 */
    m_menu->addAction(m_newFile);
    m_menu->addAction(m_openFile);
    m_menu->addAction(m_saveFile);
    m_menu->addAction(m_saveAs);
    m_menu->addSeparator();
    m_menu->addAction(m_settings);
    m_theme->addAction(m_lighttheme);
    m_theme->addAction(m_darktheme);
    m_theme->addAction(m_systemtheme);
    m_menu->addMenu(m_theme);
    m_menu->addSeparator();
    m_menu->addAction(m_about);
    m_menu->addAction(m_exit);
    titlebar()->setMenu(m_menu);

    /* 初始化保存文件成功提示信息 */
    message->setIcon(QIcon::fromTheme("dialog-ok").pixmap(64, 64));
    message->setWidget(open);
    message->hide();

    /* 页面布局 */
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
    rightLayout->addWidget(new DHorizontalLine, 5, 0, 1, 3);
    rightLayout->addWidget(new QLabel(tr("Run in Terminal")), 6, 0, 1, 2);
    rightLayout->addWidget(m_terminal, 6, 2, 1, 1);
    rightLayout->addWidget(new QLabel(tr("Not Display in Launcher")), 7, 0, 1, 2);
    rightLayout->addWidget(m_nodisplay, 7, 2, 1, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(30);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addLayout(leftLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(rightLayout);
    mainLayout->addSpacing(30);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addSpacing(20);
    layout->addLayout(mainLayout);
    layout->addSpacing(40);

    w->setLayout(layout);
}

void MainWindow::initDefaultValues()
{
    setAcceptDrops(true);                               //  允许窗口接受拖放文件
    titlebar()->setTitle(tr("Desktop Entry Editor"));   //  设置标题栏标题

    m_iconFile = nullptr;
    m_desktopFile = nullptr;
    fileName = nullptr;
    isSaveAs = false;
    m_parser = new XdgDesktopFile;                      //  清空当前文件

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
    {
        m_categories->addItem(str, Categories.value(str));
    }
    m_categories->setCurrentIndex(0);
}

void MainWindow::initConnections()
{
    connect(m_newFile, &QAction::triggered, this, [=]{newDesktopFile();});
    connect(m_openFile, &QAction::triggered, this, [=]{openDesktopFile();});
    connect(m_saveFile, &QAction::triggered, this, [=]{createOrUpdateDesktopFile();});
    connect(m_saveAs, &QAction::triggered, this, [=]{saveAsDesktopFile();});
    connect(m_settings, &QAction::triggered, this, [=]{s->show();});
    connect(m_lighttheme, &QAction::triggered, this, [=]{setBackgroundColor("Light", opacity, blur);});
    connect(m_darktheme, &QAction::triggered, this, [=]{setBackgroundColor("Dark", opacity, blur);});
    connect(m_systemtheme, &QAction::triggered, this, [=]{setBackgroundColor("", opacity, blur);});
    connect(m_about, &QAction::triggered, this, [=]{a->show();});
    connect(m_exit, &QAction::triggered, this, [=]{exitEditor();});

    //  选择不同类别后恢复窗口焦点（若点击同一个分类，会出现焦点在分类下拉框上导致快捷键无响应的问题）
    connect(m_categories, &DComboBox::currentTextChanged, this, [=]{this->setFocus();});

    connect(open, &DPushButton::clicked, this, [=]()
    {
        DDesktopServices::showFileItem(m_desktopFile);  //  在文管中标记文件位置
        message->hide();
    });

    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, [=]{setBackgroundColor(theme, opacity, blur);});
    connect(s, &Settings::opacityChanged, this, &MainWindow::setOpacity);
    connect(s, &Settings::blurChanged, this, &MainWindow::setBlur);
}

void MainWindow::loadDesktopFile()
{
    if(!m_desktopFile.isEmpty())
    {
        QFile file(m_desktopFile);
        /* 判断文件是否存在，以及文件是否可读取 */

        /*
         * 以防万一，判断文件可读方法一起改了吧......
         * 原始：file.permissions().testFlag(QFile::ReadOwner)
         *
         * 2021.04.10 02:15
         */
        if(file.exists() && QFileInfo(file).isReadable())
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

        /* 读取文件信息 */
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    Config().setSettings("settings", "opacity", opacity);
    Config().setSettings("settings", "blur", blur);
    Config().setSettings("settings", "theme", theme);
    s->close();
    a->close();
    event->accept();
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
        this->activateWindow(); //  拖入文件后窗口恢复焦点
        m_desktopFile = dropFile;
        loadDesktopFile();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    /* 点击主界面空白处获取焦点 */
    if(event->button() == Qt::LeftButton)
    {
        this->setFocus();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    /* 捕捉快捷键键并执行相应操作 */
    if (event->key() == Qt::Key_N && event->modifiers() == Qt::ControlModifier)
    {
        m_newFile->trigger();
    }
    if (event->key() == Qt::Key_O && event->modifiers() == Qt::ControlModifier)
    {
        m_openFile->trigger();
    }
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier)
    {
        m_saveFile->trigger();
    }
    if (event->key() == Qt::Key_S && event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        m_saveAs->trigger();
    }
    if (event->key() == Qt::Key_F1)
    {
        m_about->trigger();
    }
    if (event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier)
    {
        m_exit->trigger();
    }

    /* 按回车键恢复窗口焦点 */
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->setFocus();
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
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath() + SAVEPATH, tr("Desktop Entry Files (*.desktop)"));
    if(!saveFile.isEmpty())
    {
        /* 判断上层目录是否可写入 */

        /*
         * 对于文件夹的写入权限判断好像也没问题啊......
         * 原始：QFileInfo(QFileInfo(saveFile).absolutePath()).permissions().testFlag(QFile::WriteUser)
         *
         * 2021.04.10 02:24
         */
        if(QFileInfo(QFileInfo(saveFile).absolutePath()).isWritable())
        {
            m_desktopFile = saveFile;
            if(!saveFile.endsWith(".desktop"))
            {
                m_desktopFile += ".desktop";
            }
            /*
             * 不知道 20.2 还是更早之前开始，
             * 文件保存对话框写入文件名好像会自动加上后缀名了？
             *
             * 导致这里对于是否另存为操作的判断要放到外面才生效，
             * 否则会一直判断为文件无写入权限而重复显示对话框。
             *
             * 果然是我之前写的一直有 BUG 现在才注意到？
             * 但是没理由啊，测试了这么多版本了都......
             * 所以还是因为文管改了什么东西？
             *
             * 还好今天重装系统用了一次自己写的玩意儿，
             * 不然死都发现不了这问题了......
             *
             * 2020.04.10 01:50
             */
            isSaveAs = true;    //  另存为文件时文件不存在，所以无法写入，需要判断

            createOrUpdateDesktopFile();
        }
        else
        {
            saveAsDesktopFile();
            return;
        }
    }
}

void MainWindow::setOpacity(int value)
{
    opacity = value;
    setBackgroundColor(theme, opacity, blur);
}

void MainWindow::setBlur(bool flag)
{
    blur = flag;
    setBackgroundColor(theme, opacity, blur);
}

void MainWindow::setBackgroundColor(QString str, int value, bool flag)
{
    theme = str;
    opacity = value;
    blur = flag;

    QPalette palette;
    /*
     * 设置背景图片
     * palette.setBrush(QPalette::Background, QBrush(QPixmap(":/background.png")));
     */

    if(theme == "Light")
    {
        m_lighttheme->setChecked(true);
        m_darktheme->setChecked(false);
        m_systemtheme->setChecked(false);
        m_theme->update();

        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);

        palette.setColor(QPalette::Background, QColor(240, 240, 240, opacity));
    }
    else if(theme == "Dark")
    {
        m_lighttheme->setChecked(false);
        m_darktheme->setChecked(true);
        m_systemtheme->setChecked(false);
        m_theme->update();

        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);

        palette.setColor(QPalette::Background, QColor(40, 40, 40, opacity));
    }
    else
    {
        m_lighttheme->setChecked(false);
        m_darktheme->setChecked(false);
        m_systemtheme->setChecked(true);
        m_theme->update();

        DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::UnknownType);

        if(DGuiApplicationHelper::instance()->themeType() == DGuiApplicationHelper::LightType)
        {
            palette.setColor(QPalette::Background, QColor(240, 240, 240, opacity)); //  设置 RGBA
        }
        else
        {
            palette.setColor(QPalette::Background, QColor(40, 40, 40, opacity));
        }
    }
    w->setPalette(palette);
    //  titlebar()->setPalette(palette);

    setEnableBlurWindow(blur);

    update();
}

void MainWindow::chooseIcon()
{
    QString iconFile = QFileDialog::getOpenFileName(this, tr("Select icon"), QDir::homePath(), tr("Image Files (*.png *.jpg *.svg *.xpm)"));
    if(!iconFile.isEmpty())
    {
        m_iconFile = iconFile;
        m_icon->setPixmap(QIcon(m_iconFile).pixmap(128, 128));
    }
}

void MainWindow::createOrUpdateDesktopFile()
{
    /* 判断文件路径是否为空，原文件是否可写入，是否为另存为操作 */

    /*
     * 此处原始判断文件是否可写入方法为：
     * QFile::permissions(m_desktopFile).testFlag(QFile::WriteOwner)
     * 似乎不正确了，比如读取 /usr/share/applications 里面的 .desktop 文件，
     * 会显示有写入权限，然而实际并没有......
     *
     * 换用以下方式判断结果正常：
     * QFileInfo(m_desktopFile).isWritable()
     * 如果是我写的 BUG，那之前为啥没测试出来呢？
     *
     * 真要猜的话，个人感觉，
     * 有点像是......Qt 5.11.3 到 5.15.1 的　API 有变化导致的？
     *
     * 2021.04.10 02:08
     */
    if(m_desktopFile.isEmpty() || (!QFileInfo(m_desktopFile).isWritable() && !isSaveAs))
    {
        saveAsDesktopFile();
        return;
    }

    m_parser->setValue(KeyEncoding, "UTF-8");
    m_parser->setValue(KeyType, "Application");
    m_parser->setValue(KeyName, m_nameEdit->text().toUtf8());
    m_parser->setValue(KeyExec, m_execEdit->text().toUtf8());
    m_parser->setValue(KeyIcon, m_iconFile);

    if(!m_commentEdit->text().isEmpty())
    {
        m_parser->setValue(KeyComment, m_commentEdit->text().toUtf8());
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
        m_parser->setValue(KeyTerminal, "true");
    }
    else
    {
        m_parser->setValue(KeyTerminal, "false");
    }

    if(m_nodisplay->isChecked())
    {
        m_parser->setValue(KeyNoDisplay, "true");
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
        m_parser->setValue(KeyStartupWMClass, m_nameEdit->text().toUtf8());
    }

    m_parser->setValue(KeyMimeType, m_mimetypeEdit->text());

    m_parser->save(m_desktopFile);

    fileName = QFileInfo(m_desktopFile).fileName();
    titlebar()->setTitle(fileName); //  标题栏显示当前文件名

    /* 提示信息 */
    message->setMessage(QString("%1 %2 %3").arg(fileName).arg(tr("is saved.")).arg(tr("Show in file manager?")));
    DMessageManager::instance()->sendMessage(this, message);
    message->show();
}

void MainWindow::exitEditor()
{
    this->close();
}
