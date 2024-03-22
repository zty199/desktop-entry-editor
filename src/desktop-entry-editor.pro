#-------------------------------------------------
#
# Project created by QtCreator 2020-10-19T17:52:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = desktop-entry-editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Get build version from qmake
VERSION = $$BUILD_VERSION
isEmpty(VERSION): VERSION = 1.0
DEFINES += APP_VERSION=\\\"'$${VERSION}'\\\"

# Disable qWarning / qDebug output in Release
#CONFIG(release, debug | release): DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget Qt5Xdg

SOURCES += \
    main.cpp \
    application.cpp \
    mainwindow.cpp \
    utils.cpp

HEADERS += \
    application.h \
    mainwindow.h \
    utils.h \
    consts.h

TRANSLATIONS += \
    $${PWD}/../translations/$${TARGET}_es_ES.ts \
    $${PWD}/../translations/$${TARGET}_zh_CN.ts


# Update translation files
CONFIG(release, debug | release) {
    !system(bash $${PWD}/../translate_update.sh $${TARGET}): error("Failed to lupdate")
    !system(bash $${PWD}/../translate_generation.sh $${TARGET}): error("Failed to lrelease")
}

# Rules for deployment
linux {
isEmpty(PREFIX): PREFIX = /usr

credits.files += $${PWD}/../pkg/entries/deepin/credits/$${TARGET}.json
credits.path = $${PREFIX}/share/deepin/credits

desktop.files += $${PWD}/../pkg/entries/applications/$${TARGET}.desktop
desktop.path = $${PREFIX}/share/applications

icon.files += $${PWD}/../pkg/entries/icons/hicolor/scalable/apps/$${TARGET}.svg
icon.path = $${PREFIX}/share/icons/hicolor/scalable/apps

qm.files += $${PWD}/../translations/*.qm
qm.path = $${PREFIX}/share/$${TARGET}/translations

INSTALLS += \
    credits \
    desktop \
    icon \
    qm

target.path = $${PREFIX}/bin
!isEmpty(target.path): INSTALLS += target
}
