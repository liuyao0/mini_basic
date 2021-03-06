#IF "help.txt" CAN'T BE OPENED
#PLEASE CHANGE THE WORKING DIRCTORY TO ./PROJ1
#AND DON'T USE CHINESE PATH NAME!!

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Buffer.cpp \
    exp.cpp \
    main.cpp \
    program.cpp \
    statement.cpp \
    tokenizer.cpp \
    widget.cpp

HEADERS += \
    Buffer.h \
    exp.h \
    program.h \
    statement.h \
    tokenizer.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
