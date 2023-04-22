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
    av_log_set_level(AV_LOG_DEBUG) ;
    //初始化Frames
    if(! m_frames.init()){
        qDebug()<<"frames init failed";
        return;
    }

    decoder.setFrames(&m_frames);

    connect(&decoder, &VTDecoder::onNewFrame, this, [this](){
        if (ui->video_widget->isHidden()) {
            ui->video_widget->show();
        }
        m_frames.lock();
        const AVFrame *frame = m_frames.consumeRenderedFrame();
//        qDebug() << "widthxheight:" << frame->width << "x" << frame->height;
        updateShowSize(QSize(frame->width, frame->height));
        ui->video_widget->setFrameSize(QSize(frame->width, frame->height));
        ui->video_widget->updateTextures(frame->data[0], frame->data[1], frame->data[2], frame->linesize[0], frame->linesize[1], frame->linesize[2]);
        m_frames.unLock();
    },Qt::QueuedConnection);

    updateShowSize(size());

    if(decoder.init()){
        qDebug()<<"decoder init success";
        const char* path = "D:/1989.mp4";
        decoder.setFilePath(path);
        decoder.startDecode();
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateShowSize(const QSize &newSize)
{
    if (m_frameSize != newSize) {
        m_frameSize = newSize;

        bool vertical = newSize.height() > newSize.width();
        if (vertical) {
            resize(VIDEO_FROM_WIDTH, VIDEO_FROM_HEIGHT);
        } else {
            resize(VIDEO_FROM_HEIGHT, VIDEO_FROM_WIDTH);
        }
    }
}
