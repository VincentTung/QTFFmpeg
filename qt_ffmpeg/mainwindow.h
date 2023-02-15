#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <frames.h>
#include <vtdecoder.h>

#define VIDEO_FROM_WIDTH 420
#define VIDEO_FROM_HEIGHT 850

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateShowSize(const QSize &newSize);
private:
    Ui::MainWindow *ui;
    Frames    m_frames;
    VTDecoder decoder;
    QSize m_frameSize;

};
#endif // MAINWINDOW_H
