#-------------------------------------------------
#
# Project created by QtCreator 2010-12-04T20:03:07
#
#-------------------------------------------------

QT       += core gui

TARGET = f1
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    random.cpp \
    chromosom.cpp \
    parametry.cpp \
    symulacja.cpp \
    dane.cpp \
    wykres.cpp

HEADERS  += mainwindow.h \
    random.h \
    chromosom.h \
    parametry.h \
    symulacja.h \
    dane.h \
    wykres.h

FORMS    += mainwindow.ui

INCLUDEPATH += C:\Qt\2010.05\qwt-6.0.0-rc5\src
DEPENDPATH += C:\Qt\2010.05\qwt-6.0.0-rc5\lib
LIBS += -LC:\Qt\2010.05\qwt-6.0.0-rc5\lib

win32 {
     CONFIG(debug, debug|release) {
         LIBS += -lqwtd
     } else {
         LIBS += -lqwt
     }
}
