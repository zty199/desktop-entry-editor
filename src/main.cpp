#include "application.h"
#include "mainwindow.h"

#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    /**
     * NOTE: set organizationName and applicationName first
     * ensure conf files and log files are generated in right place
     */
    QCoreApplication::setOrganizationName("spark-union");
    QCoreApplication::setApplicationName("desktop-entry-editor");
    /**
     * NOTE: register log appender after applicationName is set
     */
    Application::registerLogAppender();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    Application a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QObject::tr("file"), QObject::tr("Desktop Entry file to open, optionally."));
    parser.process(a);

    MainWindow w;
    w.show();
    if (!parser.positionalArguments().isEmpty()) {
        w.setDesktopFile(parser.positionalArguments().first());
    }

    return a.exec();
}
