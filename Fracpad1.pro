#-------------------------------------------------
#
# Project created by QtCreator 2017-01-28T22:35:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fracpad1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    attrdialog.cpp \
    bezier.cpp

HEADERS  += mainwindow.h \
    attrdialog.h \
    bezier.h \
    poly.h \
    shape.h \
    myline.h

FORMS    += mainwindow.ui \
    attrdialog.ui

RESOURCES += \
    icons.qrc
