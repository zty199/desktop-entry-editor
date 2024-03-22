#include "mainwindow.h"
#include "consts.h"

#include <QMimeData>
#include <QMimeDatabase>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStandardPaths>
#include <QShortcut>

#include <DWidgetUtil>
#include <DTitlebar>
#include <DHorizontalLine>
#include <DPalette>
#if DTK_VERSION <= DTK_VERSION_CHECK(5, 5, 17, 1)
#include <DApplicationHelper>
#else
#include <DPaletteHelper>
#endif
#include <DPlatformTheme>
#include <DMessageManager>
#include <DDesktopServices>

MainWindow::MainWindow(DMainWindow *parent)
    : Dtk::Widget::DMainWindow(parent)
    , m_icon(new Dtk::Widget::DLabel(this))
    , m_iconTip(new Dtk::Widget::DLabel(this))
    , m_nameEdit(new Dtk::Widget::DLineEdit(this))
    , m_execEdit(new Dtk::Widget::DFileChooserEdit(this))
    , m_commentEdit(new Dtk::Widget::DLineEdit(this))
    , m_mimetypeEdit(new Dtk::Widget::DLineEdit(this))
    , m_categories(new Dtk::Widget::DComboBox(this))
    , m_terminal(new Dtk::Widget::DSwitchButton(this))
    , m_nodisplay(new Dtk::Widget::DSwitchButton(this))
    , message(new Dtk::Widget::DFloatingMessage(Dtk::Widget::DFloatingMessage::ResidentType, this))
    , open(new Dtk::Widget::DPushButton(QObject::tr("Open"), message))
{
    checkWorkspace();

    initUI();
    initDefaultValues();
    initConnections();
}

MainWindow::~MainWindow()
{
    delete m_parser;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if ((qobject_cast<QLabel *>(watched) == m_icon || qobject_cast<QLabel *>(watched) == m_iconTip)
        && (event->type() == QEvent::MouseButtonRelease /*&& dynamic_cast<QMouseEvent *>(event)->button() == Qt::LeftButton*/)) {
        chooseIcon();
        return true;
    }

    return Dtk::Widget::DMainWindow::eventFilter(watched, event);
}

void MainWindow::setDesktopFile(const QString &file)
{
    QMimeDatabase db;
    if (db.mimeTypeForFile(file).name() != "application/x-desktop") {
        return;
    }

    //  获取文件绝对路径
    QDir dir = QDir::currentPath();
    m_desktopFile = dir.absoluteFilePath(file);

    loadDesktopFile();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        setFocus();
    }

    Dtk::Widget::DMainWindow::mousePressEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return
        || event->key() == Qt::Key_Enter) {
        setFocus();
    }

    Dtk::Widget::DMainWindow::keyPressEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Dtk::Widget::DMainWindow::paintEvent(event);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }

    Dtk::Widget::DMainWindow::dragEnterEvent(event);
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString dropFile = event->mimeData()->urls().at(0).toLocalFile();
    QMimeDatabase db;
    if (db.mimeTypeForFile(dropFile).name() == "application/x-desktop") {
        m_desktopFile = dropFile;
        loadDesktopFile();
    }

    activateWindow();
    Dtk::Widget::DMainWindow::dropEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
#if DTK_VERSION < DTK_VERSION_CHECK(5, 5, 17, 1)
        Dtk::Gui::DPalette palette = Dtk::Widget::DApplicationHelper::instance()->palette(m_iconTip);
        palette.setColor(QPalette::WindowText, Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color());
        Dtk::Widget::DApplicationHelper::instance()->setPalette(m_iconTip, palette);
#else
        Dtk::Gui::DPalette palette = Dtk::Widget::DPaletteHelper::instance()->palette(m_iconTip);
        palette.setColor(QPalette::WindowText, Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette().highlight().color());
        Dtk::Widget::DPaletteHelper::instance()->setPalette(m_iconTip, palette);
#endif
    }

    Dtk::Widget::DMainWindow::changeEvent(event);
}

void MainWindow::checkWorkspace()
{
    QDir dir;
    dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/applications/" + qApp->applicationName());
    dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
}

void MainWindow::initUI()
{
    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    setFixedHeight(450);
    setMaximumWidth(800);
    setMinimumWidth(600);
    moveToCenter(this);

    initTitlebar();

    setCentralWidget(new QWidget(this));
    centralWidget()->setAcceptDrops(true); // 允许窗口接受拖放文件

    QHBoxLayout *layout = new QHBoxLayout(centralWidget());
    layout->setContentsMargins(50, 20, 50, 40);
    layout->setSpacing(50);
    layout->setAlignment(Qt::AlignCenter);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setMargin(0);
    leftLayout->setSpacing(10);
    leftLayout->setAlignment(Qt::AlignCenter);

    m_icon->installEventFilter(this); // 响应点击图标事件
    leftLayout->addWidget(m_icon);

    m_iconTip->installEventFilter(this);
    leftLayout->addWidget(m_iconTip);
    layout->addLayout(leftLayout);

    QGridLayout *rightLayout = new QGridLayout;
    rightLayout->setMargin(0);
    rightLayout->setSpacing(10);
    rightLayout->setColumnStretch(1, 1);
    rightLayout->addWidget(new QLabel(QObject::tr("Name")), 0, 0);
    rightLayout->addWidget(m_nameEdit, 0, 1, 1, 2);
    rightLayout->addWidget(new QLabel(QObject::tr("Exec")), 1, 0);
    rightLayout->addWidget(m_execEdit, 1, 1, 1, 2);
    rightLayout->addWidget(new QLabel(QObject::tr("Comment")), 2, 0);
    rightLayout->addWidget(m_commentEdit, 2, 1, 1, 2);
    rightLayout->addWidget(new QLabel(QObject::tr("MimeType")), 3, 0);
    rightLayout->addWidget(m_mimetypeEdit, 3, 1, 1, 2);
    rightLayout->addWidget(new QLabel(QObject::tr("Categories")), 4, 0);
    rightLayout->addWidget(m_categories, 4, 1, 1, 2);
    rightLayout->addWidget(new Dtk::Widget::DHorizontalLine, 5, 0, 1, 3);
    rightLayout->addWidget(new QLabel(QObject::tr("Run in Terminal")), 6, 0, 1, 2);
    rightLayout->addWidget(m_terminal, 6, 2, 1, 1);
    rightLayout->addWidget(new QLabel(QObject::tr("Not Display in Launcher")), 7, 0, 1, 2);
    rightLayout->addWidget(m_nodisplay, 7, 2, 1, 1);
    layout->addLayout(rightLayout);

    /* 初始化保存文件成功提示信息 */
    message->setIcon(QIcon::fromTheme("dialog-ok").pixmap(64, 64));
    message->setWidget(open);
    message->hide();
}

void MainWindow::initTitlebar()
{
    titlebar()->setIcon(QIcon::fromTheme("desktop-entry-editor"));

    QMenu *menu = new QMenu(titlebar());
    titlebar()->setMenu(menu);

    QAction *newAction = new QAction(QObject::tr("New"), menu);
    connect(newAction, &QAction::triggered, this, [=] { newDesktopFile(); });

    QAction *openAction = new QAction(QObject::tr("Open"), menu);
    connect(openAction, &QAction::triggered, this, [=] { openDesktopFile(); });

    QAction *saveAction = new QAction(QObject::tr("Save"), menu);
    connect(saveAction, &QAction::triggered, this, [=] { createOrUpdateDesktopFile(); });

    QAction *saveAsAction = new QAction(QObject::tr("Save As"), menu);
    connect(saveAsAction, &QAction::triggered, this, [=] { saveAsDesktopFile(); });

    menu->addAction(newAction);
    menu->addAction(openAction);
    menu->addAction(saveAction);
    menu->addAction(saveAsAction);

    new QShortcut(QKeySequence::New, this, SLOT(newDesktopFile()), nullptr, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence::Open, this, SLOT(openDesktopFile()), nullptr, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence::Save, this, SLOT(createOrUpdateDesktopFile()), nullptr, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence::SaveAs, this, SLOT(saveAsDesktopFile()), nullptr, Qt::ApplicationShortcut);
    new QShortcut(QKeySequence::Quit, this, SLOT(close()), nullptr, Qt::ApplicationShortcut);
}

void MainWindow::initDefaultValues()
{
    titlebar()->setTitle(QObject::tr("Desktop Entry Editor")); // 设置标题栏标题

    m_iconFile = nullptr;
    m_desktopFile = nullptr;
    fileName = nullptr;
    isSaveAs = false;

    if (m_parser) {
        delete m_parser;
        m_parser = nullptr;
    }
    m_parser = new XdgDesktopFile; // 清空当前文件

    m_icon->setPixmap(QIcon::fromTheme(m_iconFile, QIcon::fromTheme("application-x-executable")).pixmap(128, 128));

#if DTK_VERSION < DTK_VERSION_CHECK(5, 5, 17, 1)
    Dtk::Gui::DPalette palette = Dtk::Widget::DApplicationHelper::instance()->palette(m_iconTip);
    palette.setColor(QPalette::WindowText, palette.color(QPalette::Highlight));
    Dtk::Widget::DApplicationHelper::instance()->setPalette(m_iconTip, palette);
#else
    Dtk::Gui::DPalette palette = Dtk::Widget::DPaletteHelper::instance()->palette(m_iconTip);
    palette.setColor(QPalette::WindowText, palette.color(QPalette::Highlight));
    Dtk::Widget::DPaletteHelper::instance()->setPalette(m_iconTip, palette);
#endif

    m_iconTip->setText(QObject::tr("Click to choose icon"));
    m_iconTip->setAlignment(Qt::AlignCenter);

    m_nameEdit->clear();
    m_nameEdit->setPlaceholderText(QObject::tr("Name to display"));
    m_commentEdit->clear();
    m_commentEdit->setPlaceholderText(QObject::tr("Comment of this .desktop"));
    m_execEdit->clear();
    m_execEdit->setPlaceholderText(QObject::tr("Choose a binary"));
    m_mimetypeEdit->clear();
    m_mimetypeEdit->setPlaceholderText(QObject::tr("Supported MIME type(s)"));

    m_terminal->setChecked(false);
    m_nodisplay->setChecked(false);

    m_categories->clear();
    QMap<QString, QString> Categories;
    Categories.insert(QString("- %1 -").arg(QObject::tr("Please Select")), "");
    Categories.insert(QObject::tr("Network"), "Network");
    Categories.insert(QObject::tr("Chat"), "Chat");
    Categories.insert(QObject::tr("Music"), "Music");
    Categories.insert(QObject::tr("Video"), "Video");
    Categories.insert(QObject::tr("Graphics"), "Graphics");
    Categories.insert(QObject::tr("Game"), "Game");
    Categories.insert(QObject::tr("Office"), "Office");
    Categories.insert(QObject::tr("Translation"), "Translation");
    Categories.insert(QObject::tr("Development"), "Development");
    Categories.insert(QObject::tr("System"), "System");
    Categories.insert(QObject::tr("Others"), "Others");
    foreach (const QString &str, Categories.keys()) {
        m_categories->addItem(str, Categories.value(str));
    }
    m_categories->setCurrentIndex(0);
}

void MainWindow::initConnections()
{
    // 选择不同类别后恢复窗口焦点（若点击同一个分类，会出现焦点在分类下拉框上导致快捷键无响应的问题）
    connect(m_categories, &Dtk::Widget::DComboBox::currentTextChanged, this, [=] { this->setFocus(); });

    connect(open, &Dtk::Widget::DPushButton::clicked, this, [=] {
        Dtk::Widget::DDesktopServices::showFileItem(m_desktopFile); // 在文管中标记文件位置
        message->hide();
    });

    connect(Dtk::Gui::DGuiApplicationHelper::instance()->applicationTheme(),
            &Dtk::Gui::DPlatformTheme::iconThemeNameChanged, this, &MainWindow::slotIconThemeNameChanged);
}

void MainWindow::loadDesktopFile()
{
    if (!m_desktopFile.isEmpty()) {
        QFile file(m_desktopFile);
        /* 判断文件是否存在，以及文件是否可读取 */
        if (file.exists() && QFileInfo(file).isReadable()) {
            fileName = QFileInfo(file).fileName(); // 获取文件名
            titlebar()->setTitle(fileName); // 显示当前文件名
            m_parser->load(QFileInfo(file).absoluteFilePath()); // 载入文件（会自动清空原有键值）
            m_desktopFile = QFileInfo(file).absoluteFilePath(); // 更新文件绝对路径
        } else {
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
        if (m_parser->value(KeyCategories).toString().isEmpty()) {
            Categories.insert(QString("- %1 -").arg(QObject::tr("Please Select")), "");
        } else {
            Categories.insert(m_parser->value(KeyCategories).toString(), m_parser->value(KeyCategories).toString());
        }
        Categories.insert(QObject::tr("Network"), "Network");
        Categories.insert(QObject::tr("Chat"), "Chat");
        Categories.insert(QObject::tr("Music"), "Music");
        Categories.insert(QObject::tr("Video"), "Video");
        Categories.insert(QObject::tr("Graphics"), "Graphics");
        Categories.insert(QObject::tr("Game"), "Game");
        Categories.insert(QObject::tr("Office"), "Office");
        Categories.insert(QObject::tr("Translation"), "Translation");
        Categories.insert(QObject::tr("Development"), "Development");
        Categories.insert(QObject::tr("System"), "System");
        Categories.insert(QObject::tr("Others"), "Others");
        foreach (const QString &str, Categories.keys()) {
            m_categories->addItem(str, Categories.value(str));
        }
        m_categories->setCurrentIndex(0);

        m_terminal->setChecked(m_parser->value(KeyTerminal).toBool());
        m_nodisplay->setChecked(m_parser->value(KeyNoDisplay).toBool());

        m_iconFile = m_parser->value(KeyIcon).toString();
        m_icon->setPixmap(QIcon::fromTheme(m_iconFile, QIcon::fromTheme("application-x-executable")).pixmap(128, 128));
    }
}

void MainWindow::slotIconThemeNameChanged(const QByteArray &iconThemeName)
{
    Q_UNUSED(iconThemeName)
    m_icon->setPixmap(QIcon::fromTheme(m_iconFile, QIcon::fromTheme("application-x-executable")).pixmap(128, 128));
}

void MainWindow::newDesktopFile()
{
    initDefaultValues();
}

void MainWindow::openDesktopFile()
{
    QString openFile = QFileDialog::getOpenFileName(this, QObject::tr("Select file"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    QObject::tr("All Files (*)"));
    QMimeDatabase db;
    if (db.mimeTypeForFile(openFile).name() == "application/x-desktop") {
        m_desktopFile = openFile;
        loadDesktopFile();
    }
}

void MainWindow::saveAsDesktopFile()
{
    QString saveFile = QFileDialog::getSaveFileName(this, QObject::tr("Save As"),
                                                    QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/applications/" + qApp->applicationName(),
                                                    QObject::tr("Desktop Entry Files (*.desktop)"));
    if (saveFile.isEmpty()) {
        return;
    }

    /* 判断上层目录是否可写入 */
    if (!QFileInfo(QFileInfo(saveFile).absolutePath()).isWritable()) {
        saveAsDesktopFile();
        return;
    }

    m_desktopFile = saveFile;
    if (!saveFile.endsWith(".desktop")) {
        m_desktopFile += ".desktop";
    }

    isSaveAs = true; // 另存为文件时文件不存在，所以无法写入，需要判断

    createOrUpdateDesktopFile();
}

void MainWindow::chooseIcon()
{
    QString iconFile = QFileDialog::getOpenFileName(this, QObject::tr("Select icon"),
                                                    QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                    QObject::tr("Image Files (*.png *.jpg *.jpeg *.svg *.xpm)"));
    if (!iconFile.isEmpty()) {
        m_iconFile = iconFile;
        m_icon->setPixmap(QIcon::fromTheme(m_iconFile, QIcon::fromTheme("application-x-executable")).pixmap(128, 128));
    }
}

void MainWindow::createOrUpdateDesktopFile()
{
    /* 判断文件路径是否为空，原文件是否可写入，是否为另存为操作 */
    if (m_desktopFile.isEmpty()
        || (!QFileInfo(m_desktopFile).isWritable() && !isSaveAs)) {
        saveAsDesktopFile();
        return;
    }

    m_parser->setValue(KeyEncoding, "UTF-8");
    m_parser->setValue(KeyType, "Application");
    m_parser->setValue(KeyName, m_nameEdit->text().toUtf8());
    m_parser->setValue(KeyExec, m_execEdit->text().toUtf8());
    m_parser->setValue(KeyIcon, m_iconFile);

    if (!m_commentEdit->text().isEmpty()) {
        m_parser->setValue(KeyComment, m_commentEdit->text().toUtf8());
    } else {
        m_parser->removeEntry(KeyComment);
    }

    if (m_categories->currentIndex() == 0 && !m_categories->currentData().toString().isEmpty()) {
        m_parser->setValue(KeyCategories, m_categories->currentData());
    } else if (m_categories->currentIndex() != 0) {
        m_parser->setValue(KeyCategories, m_categories->currentData().toString().append(";"));
    } else {
        m_parser->removeEntry(KeyCategories);
    }

    m_parser->setValue(KeyTerminal, m_terminal->isChecked() ? "true" : "false");

    if (m_nodisplay->isChecked()) {
        m_parser->setValue(KeyNoDisplay, "true");
    } else {
        m_parser->removeEntry(KeyNoDisplay);
    }

    if (m_parser->value(KeyStartupWMClass).toString().isEmpty() && m_nameEdit->text().isEmpty()) {
        m_parser->removeEntry(KeyStartupWMClass);
    } else {
        m_parser->setValue(KeyStartupWMClass, m_nameEdit->text().toUtf8());
    }

    m_parser->setValue(KeyMimeType, m_mimetypeEdit->text());

    m_parser->save(m_desktopFile);

    fileName = QFileInfo(m_desktopFile).fileName();
    titlebar()->setTitle(fileName); //  标题栏显示当前文件名

    /* 提示信息 */
    QString message_1 = QObject::tr("%1 is saved.").arg(fileName);
    QString message_2 = QObject::tr("Show in file manager?");
    message->setMessage(QString("%1 %2").arg(message_1).arg(message_2));
    Dtk::Widget::DMessageManager::instance()->sendMessage(window(), message);
    message->show();
}
