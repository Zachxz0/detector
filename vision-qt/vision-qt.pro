#-------------------------------------------------
#
# Project created by QtCreator 2017-04-05T12:33:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vision-qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS USE_OPENCV

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += -I/usr/local/include

SOURCES +=  zoson.pb.cc \
    main.cpp\
    mainwindow.cpp \
    net.cpp \
    net_thread.cpp \
    detectorproxy.cpp

HEADERS  += zoson.pb.h \
    mainwindow.h \
    qpixmap_item.h \
    net.hpp \
    net_thread.h \
    detectorproxy.h


FORMS    += mainwindow.ui

LIBS += -L/usr/lib/ -lprotobuf -lopencv_core -lopencv_imgproc -lopencv_highgui
