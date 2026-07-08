QT += testlib gui widgets
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../VectorEditor

SOURCES += tst_checkcalculations.cpp \
    D:/QT/projects/VectorEditor/drawingscene.cpp
HEADERS += \
    D:/QT/projects/VectorEditor/drawingscene.h