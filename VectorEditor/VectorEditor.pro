QT += core gui widgets

TARGET = VectorEditor
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    drawingscene.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    drawingscene.h \
    mainwindow.h


FORMS += \
    mainwindow.ui