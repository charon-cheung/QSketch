#-------------------------------------------------
#
# Project created by QtCreator 2017-08-02T20:48:06
#
#-------------------------------------------------

QT       += core gui printsupport svg
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter
QMAKE_CFLAGS_WARN_OFF += -Wreturn-type
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11     # 有的编译器不添加会在lambda语法上报错
TARGET = QSketch
TEMPLATE = app

RC_ICONS = Draw.ico

include (Items/Items.pri)
include (Dialogs/Dialogs.pri)

DESTDIR = bin

CONFIG(debug, debug | release){
    compiled = debug
}
CONFIG(release, debug | release){
    compiled = release
}

UI_DIR = $$compiled/ui
MOC_DIR = $$compiled/moc
OBJECTS_DIR = $$compiled/obj
RCC_DIR = $$compiled/res
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        myview.cpp \
        myscene.cpp \
        QRecentFilesMenu.cpp \
        command.cpp \
        clock.cpp \

HEADERS += \
        mainwindow.h \
        myview.h \
        myscene.h \
        QRecentFilesMenu.h \
        command.h \
        clock.h \

FORMS += \
        mainwindow.ui \


RESOURCES += \
    res.qrc

DISTFILES +=
