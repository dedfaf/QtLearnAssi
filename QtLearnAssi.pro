QT += core gui multimedia multimediawidgets widgets quickwidgets location network charts
QT += core gui webenginewidgets

RESOURCES += icon.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatwindow.cpp \
    func_browser.cpp \
    func_image.cpp \
    func_map.cpp \
    func_music.cpp \
    func_video.cpp \
    func_weather.cpp \
    main.cpp \
    mainwindow.cpp \
    timedisplay.cpp

HEADERS += \
    chatwindow.h \
    func_browser.h \
    func_image.h \
    func_map.h \
    func_music.h \
    func_video.h \
    func_weather.h \
    mainwindow.h \
    timedisplay.h

FORMS += \
    chatwindow.ui \
    func_browser.ui \
    func_image.ui \
    func_map.ui \
    func_music.ui \
    func_video.ui \
    func_weather.ui \
    mainwindow.ui \
    timedisplay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../white/white/0@1x.png \
    ../white/white/0@2x.png \
    ../white/white/10@1x.png \
    ../white/white/10@2x.png \
    ../white/white/11@1x.png \
    ../white/white/11@2x.png \
    ../white/white/12@1x.png \
    ../white/white/12@2x.png \
    ../white/white/13@1x.png \
    ../white/white/13@2x.png \
    ../white/white/14@1x.png \
    ../white/white/14@2x.png \
    ../white/white/15@1x.png \
    ../white/white/15@2x.png \
    ../white/white/16@1x.png \
    ../white/white/16@2x.png \
    ../white/white/17@1x.png \
    ../white/white/17@2x.png \
    ../white/white/18@1x.png \
    ../white/white/18@2x.png \
    ../white/white/19@1x.png \
    ../white/white/19@2x.png \
    ../white/white/1@1x.png \
    ../white/white/1@2x.png \
    ../white/white/20@1x.png \
    ../white/white/20@2x.png \
    ../white/white/21@1x.png \
    ../white/white/21@2x.png \
    ../white/white/22@1x.png \
    ../white/white/22@2x.png \
    ../white/white/23@1x.png \
    ../white/white/23@2x.png \
    ../white/white/24@1x.png \
    ../white/white/24@2x.png \
    ../white/white/25@1x.png \
    ../white/white/25@2x.png \
    ../white/white/26@1x.png \
    ../white/white/26@2x.png \
    ../white/white/27@1x.png \
    ../white/white/27@2x.png \
    ../white/white/28@1x.png \
    ../white/white/28@2x.png \
    ../white/white/29@1x.png \
    ../white/white/29@2x.png \
    ../white/white/2@1x.png \
    ../white/white/2@2x.png \
    ../white/white/30@1x.png \
    ../white/white/30@2x.png \
    ../white/white/31@1x.png \
    ../white/white/31@2x.png \
    ../white/white/32@1x.png \
    ../white/white/32@2x.png \
    ../white/white/33@1x.png \
    ../white/white/33@2x.png \
    ../white/white/34@1x.png \
    ../white/white/34@2x.png \
    ../white/white/35@1x.png \
    ../white/white/35@2x.png \
    ../white/white/36@1x.png \
    ../white/white/36@2x.png \
    ../white/white/37@1x.png \
    ../white/white/37@2x.png \
    ../white/white/38@1x.png \
    ../white/white/38@2x.png \
    ../white/white/3@1x.png \
    ../white/white/3@2x.png \
    ../white/white/4@1x.png \
    ../white/white/4@2x.png \
    ../white/white/5@1x.png \
    ../white/white/5@2x.png \
    ../white/white/6@1x.png \
    ../white/white/6@2x.png \
    ../white/white/7@1x.png \
    ../white/white/7@2x.png \
    ../white/white/8@1x.png \
    ../white/white/8@2x.png \
    ../white/white/99@1x.png \
    ../white/white/99@2x.png \
    ../white/white/9@1x.png \
    ../white/white/9@2x.png

RESOURCES += \
    icon.qrc
    Map.qml

RESOURCES += \
    Qml.qrc
