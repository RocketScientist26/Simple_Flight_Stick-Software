#Qt
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS

#C++
CONFIG += c++17

#Files
LIBS += \
        -L$$_PRO_FILE_PWD_/Hidapi -lhidapi -lsetupapi

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        Hidapi/hidapi.h \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += \
        default.qrc

#Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
