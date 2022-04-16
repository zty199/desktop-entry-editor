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

# 禁止输出 qWarning / qDebug 信息
CONFIG(release, debug|release): DEFINES += QT_NO_WARNING_OUTPUT QT_NO_DEBUG_OUTPUT

CONFIG += c++11 link_pkgconfig

PKGCONFIG += dtkwidget Qt5Xdg

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        config.cpp \
        settings.cpp \
        about.cpp

HEADERS += \
        mainwindow.h \
        config.h \
        settings.h \
        about.h

FORMS += \
      settings.ui

TRANSLATIONS += \
             translations/desktop-entry-editor_zh_CN.ts \
             translations/desktop-entry-editor_es_ES.ts

# Automating generation .qm files from .ts files
!system($$PWD/translations/translate_generation.sh): error("Failed to generate translation")

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
