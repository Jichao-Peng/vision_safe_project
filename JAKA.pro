#-------------------------------------------------
#
# Project created by Leo 2018/3
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JAKA
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

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so\
        /usr/local/lib/libopencv_shape.so \
        /usr/local/lib/libopencv_videoio.so \
        /usr/local/lib/libopencv_core.so.3.0 \

SOURCES += main.cpp\
        jaka.cpp \
    jakasetroi.cpp \
    jakasafe.cpp \
    jakavisionpro.cpp \
    jakasetup.cpp \
    vibe-background-sequential.cpp \
    jakacomm.cpp

HEADERS  += jaka.h \
    jakasetroi.h \
    jakasafe.h \
    jakavisionpro.h \
    jakasetup.h \
    vibe-background-sequential.h \
    jakacomm.h

FORMS    += jaka.ui \
    jakasetup.ui

RESOURCES += \
    log.qrc
