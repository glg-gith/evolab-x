#-------------------------------------------------
#
# Project created by QtCreator 2014-05-13T22:04:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EVOLAB-QT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    varlistitem.cpp \
    lifecyclelistitem.cpp \
    XMLUtils.cpp \
    elpsversion.cpp \
    popqlistwidget.cpp \
    EventFilters/MouseAndKeyPressOnlyFilter.cpp \
    EventFilters/KeyPressOnlyFilter.cpp \
    ExceptionCatchingQApplication.cpp \
    Models/Macros.cpp \
    Coding/Highlighter.cpp \
    Coding/LineNumberArea.cpp \
    qtexteditcodeeditor.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    varlistitem.h \
    lifecyclelistitem.h \
    Consts.h \
    XMLUtils.h \
    elpsversion.h \
    Models/PointerLessModels.h \
    popqlistwidget.h \
    Q_DebugStream.h \
    EventFilters/KeyPressOnlyFilter.h \
    EventFilters/MouseAndKeyPressOnlyFilter.h \
    ExceptionCatchingQApplication.h \
    Models/Macros.h \
    Coding/Highlighter.h \
    StringUtils.h \
    Coding/LineNumberArea.h \
    qtexteditcodeeditor.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    varlistitem.ui \
    lifecyclelistitem.ui \
    elpsversion.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../DEPS/lib/release/ -lelp -ldl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../DEPS/lib/debug/ -lelp -ldl
//else:unix: LIBS += -L$$PWD/../DEPS/lib/ -lelp -ldl
else:unix: LIBS += -L$$PWD/../DEPS/lib/ -lgsl -lelp -ldl

INCLUDEPATH += $$PWD/../DEPS/inc
DEPENDPATH += $$PWD/../DEPS/inc

OTHER_FILES +=

RESOURCES += \
    Resources/Resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/release/ -lxml2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/debug/ -lxml2
else:unix: LIBS += -L$$PWD/../../../../../../usr/lib/x86_64-linux-gnu/ -lxml2

#INCLUDEPATH += $$PWD/../../../../../../usr/include/libxml2
#DEPENDPATH += $$PWD/../../../../../../usr/include/libxml2
INCLUDEPATH += /usr/include/libxml2
DEPENDPATH += /usr/include/libxml2

##QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -fPIC
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-sign-compare
QMAKE_CXXFLAGS -= -Wall
#QMAKE_CXXFLAGS += -rdynamic -ansi -pedantic

QMAKE_LFLAGS += -ldl




