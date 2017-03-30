#-------------------------------------------------
#
# Project created by QtCreator 2017-03-23T18:54:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = idinfo
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


INCLUDEPATH += D:\opencv\build\include
INCLUDEPATH += D:\opencv\build\include\opencv\
INCLUDEPATH += D:\opencv\build\include\opencv2\
INCLUDEPATH += C:\tesseract-build\include
INCLUDEPATH += C:\Users\weiyi\Desktop\SQL\include
INCLUDEPATH += C:\Users\weiyi\Desktop\boost_1_63_0

LIBS += -LD:\opencv\build\x86\vc12\lib
-opencv_ml2413d.lib
-opencv_calib3d2413d.lib
-opencv_contrib2413d.lib
-opencv_core2413d.lib
-opencv_features2d2413d.lib
-opencv_flann2413d.lib
-opencv_gpu2413d.lib
-opencv_highgui2413d.lib
-opencv_imgproc2413d.lib
-opencv_legacy2413d.lib
-opencv_objdetect2413d.lib
-opencv_ts2413d.lib
-opencv_video2413d.lib
-opencv_nonfree2413d.lib
-opencv_ocl2413d.lib
-opencv_photo2413d.lib
-opencv_stitching2413d.lib
-opencv_superres2413d.lib
-opencv_videostab2413d.lib



HEADERS  += \
    login_dialog.h \
    mainwindow.h \
    set_dialog.h




FORMS    += \
    login_dialog.ui \
    mainwindow.ui \
    set_dialog.ui


DISTFILES +=


SOURCES += \
    login_dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    set_dialog.cpp
