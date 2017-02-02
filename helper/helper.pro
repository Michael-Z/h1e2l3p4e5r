#-------------------------------------------------
#
# Project created by QtCreator 2017-01-27T14:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = helper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \    
    glob.cpp \
    app.cpp \
    mac_utility.cpp \
    mgc_element.cpp \
    mgc_recognizer.cpp \
    mgc_screen.cpp \
    mgc_table.cpp \
    mgc_player.cpp \
    mgc_card.cpp \
    AddNewMaskHelper.cpp

HEADERS  += mainwindow.h \    
    deps.h \
    glob.h \
    app.h \
    mac_utility.h \
    mgc_element.h \
    mgc_recognizer.h \
    mgc_screen.h \
    mgc_table.h \
    mgc_player.h \
    mgc_card.h \
    AddNewMaskHelper.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -LC:/opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/opencv/build/x64/vc14/lib/ -lopencv_world320d

INCLUDEPATH += C:/opencv/build/include
DEPENDPATH += C:/opencv/build/include
