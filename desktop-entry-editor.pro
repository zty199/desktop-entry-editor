#-------------------------------------------------
#
# Project created by QtCreator 2024-03-22T09:00:00
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += \
    src/desktop-entry-editor.pro

CONFIG += ordered
DEFINES += QT_MESSAGELOGCONTEXT

# Rules for deployment
desktop.files += pkg/entries/applications/desktop-entry-editor.desktop
desktop.path = /usr/share/applications

icon.files += pkg/entries/icons/hicolor/scalable/apps/desktop-entry-editor.svg
icon.path = /usr/share/icons/hicolor/scalable/apps

qm.files += translations/*.qm
qm.path = /usr/share/desktop-entry-editor/translations

INSTALLS += \
    desktop \
    icon \
    qm
