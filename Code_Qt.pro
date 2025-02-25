QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Cuaso.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Cuaso.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

QXLSX_PARENTPATH = D:/Code_Qt/QXlsx/QXlsx  # Đường dẫn đến thư viện
QXLSX_HEADERPATH = $$QXLSX_PARENTPATH/header/  # Thư mục chứa file .h
QXLSX_SOURCEPATH = $$QXLSX_PARENTPATH/source/  # Thư mục chứa mã nguồn .cpp
include($$QXLSX_PARENTPATH/QXlsx.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DEFINES += PROJECT_PATH=\\\"$$PWD\\\"

