QT       += core gui sql qml quick quickcontrols2 charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Cuaso.cpp \
    Translation.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Cuaso.h \
    Translation.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Cấu hình QXlsx: Hãy đảm bảo rằng đường dẫn thư viện QXlsx phù hợp với nền tảng
win32 {
    QXLSX_PARENTPATH = C:\Users\tranv\OneDrive\Desktop\QXlsx\QXlsx
}
android {
    # Đối với Android, bạn có thể cần đặt lại đường dẫn thư viện nếu có
    QXLSX_PARENTPATH = $$PWD/../QXlsx/QXlsx
}
ios {
    # Tương tự, cấu hình đường dẫn cho iOS
    QXLSX_PARENTPATH = $$PWD/../QXlsx/QXlsx
}

QXLSX_HEADERPATH = $$QXLSX_PARENTPATH/header/
QXLSX_SOURCEPATH = $$QXLSX_PARENTPATH/source/
include($$QXLSX_PARENTPATH/QXlsx.pri)

# Quy tắc cài đặt mặc định
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
DEFINES += PROJECT_PATH=\\\"$$PWD\\\"

TRANSLATIONS = app_vi.ts app_en.ts app_fr.ts

DISTFILES +=  # Lưu ý: bạn có thể cập nhật phần này nếu cần thêm file cho từng nền tảng
