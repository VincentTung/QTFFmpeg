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
       //初始化Frames
     m_frames.init();
     qDebug()<<avcodec_configuration();
     VTDecoder decoder =  VTDecoder();
     decoder.setFrames(&m_frames);
     if(decoder.init()){
        qDebug()<<"decoder init success";
        const char* path = "D:/1988.mp4";
        decoder.openFile(path);
     }

     connect(&decoder, &VTDecoder::onNewFrame, this, [this](){
            if (ui->video_widget->isHidden()) {
//                ui->loadingWidget->close();
                ui->video_widget->show();
            }
            m_frames.lock();
            const AVFrame *frame = m_frames.consumeRenderedFrame();
            //qDebug() << "widthxheight:" << frame->width << "x" << frame->height;
//            updateShowSize(QSize(frame->width, frame->height));
            ui->video_widget->setFrameSize(QSize(frame->width, frame->height));
            ui->video_widget->updateTextures(frame->data[0], frame->data[1], frame->data[2], frame->linesize[0], frame->linesize[1], frame->linesize[2]);
            m_frames.unLock();
        },Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

