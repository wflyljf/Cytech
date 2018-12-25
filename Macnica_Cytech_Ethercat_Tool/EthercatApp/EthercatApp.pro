#-------------------------------------------------
#
# Project created by QtCreator 2018-08-09T14:27:00
#
#-------------------------------------------------

QT       += core gui serialport printsupport


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Macnica_Cytech_Ethercat_Tool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    windowswitch.cpp \
    windowdock.cpp \
    serialdialog.cpp \
    serialctrl.cpp \
    alctrl.cpp \
    speedctrl.cpp \
    qcustomplot.cpp \
    Msg.c \
    plotshow.cpp \
    filectrl.cpp \
    ctrlfiledialog.cpp \
    ploatthread.cpp \
    ethercatmsg.cpp \
    speedplot.cpp \
    posplot.cpp \
    crc32c.c

HEADERS  += mainwindow.h \
    windowswitch.h \
    windowdock.h \
    serialdialog.h \
    serialctrl.h \
    alctrl.h \
    speedctrl.h \
    qcustomplot.h \
    Debug.h \
    Type.h \
    Msg.h \
    plotshow.h \
    filectrl.h \
    ctrlfiledialog.h \
    ploatthread.h \
    ethercatmsg.h \
    speedplot.h \
    posplot.h \
    crc32c.h

FORMS += \
    speedplot.ui \
    posplot.ui

#INCLUDEPATH += -I  C:\Users\jonasli\AppData\Local\Programs\Python\Python37-32\include

#LIBS += -L C:\Users\jonasli\AppData\Local\Programs\Python\Python37-32\libs -lpython37


OTHER_FILES +=
