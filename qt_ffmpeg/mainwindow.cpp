#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
    #include <libavformat/version.h>
    #include <libavutil/time.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/imgutils.h>
}
#include <QDebug>
#include "vtdecoder.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     qDebug()<<avcodec_configuration();
     VTDecoder decoder =  VTDecoder();
     if(decoder.init()){
        qDebug()<<"decoder init success";
        const char* path = "D:/1988.mp4";
        decoder.openFile(path);
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

