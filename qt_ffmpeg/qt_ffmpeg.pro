QT       += core gui opengl
QT       += openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fpscounter.cpp \
    frames.cpp \
    main.cpp \
    mainwindow.cpp \
    qyuvopenglwidget.cpp \
    vtdecoder.cpp

HEADERS += \
    fpscounter.h \
    frames.h \
    mainwindow.h \
    qyuvopenglwidget.h \
    vtdecoder.h

FORMS += \
    mainwindow.ui


INCLUDEPATH += $$PWD/third_party/ffmpeg/include

#LIBS += $$PWD/third_party/ffmpeg/lib/avcodec.lib  \
#        $$PWD/third_party/ffmpeg/lib/avdevice.lib  \
#        $$PWD/third_party/ffmpeg/lib/avfilter.lib  \
#        $$PWD/third_party/ffmpeg/lib/avformat.lib  \
#        $$PWD/third_party/ffmpeg/lib/avutil.lib  \
#        $$PWD/third_party/ffmpeg/lib/postproc.lib  \
#        $$PWD/third_party/ffmpeg/lib/swresample.lib  \
#        $$PWD/third_party/ffmpeg/lib/swscale.lib  \



# *****************************************************
# Win平台下配置
# *****************************************************
win32 {

# 依赖模块
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lavcodec-60
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lavdevice-60
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lavfilter-9
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lavformat-60
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lavutil-58
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lpostproc-57
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lswresample-4
LIBS += -L$$PWD/third_party/ffmpeg/bin/ -lswscale-7
}

# *****************************************************
# Mac平台下配置
# *****************************************************
macos {

# 依赖模块
LIBS += \
        -L$$PWD/thrid_party/ffmpeg/lib -lavformat.60\
        -L$$PWD/thrid_party/ffmpeg/lib -lavcodec.60 \
        -L$$PWD/thrid_party/ffmpeg/lib -lavutil.56 \
        -L$$PWD/thrid_party/ffmpeg/lib -lswscale.7 \
}

## *****************************************************
## Linux平台下配置
## *****************************************************
#linux {
## 依赖模块
#LIBS += \
#        -L$$PWD/thrid_party/ffmpeg/lib -lavformat \
#        -L$$PWD/thrid_party/ffmpeg/lib -lavcodec \
#        -L$$PWD/thrid_party/ffmpeg/lib -lavutil \
#        -L$$PWD/thrid_party/ffmpeg/lib -lswscale \
#}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
