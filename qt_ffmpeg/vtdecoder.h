#ifndef VTDECODER_H
#define VTDECODER_H
#include <QThread>
#include <frames.h>
class VTDecoder:public QThread
{
 Q_OBJECT
public:
    VTDecoder();
   bool init();
   void setFilePath(const char* filePath);
   void setFrames(Frames * frames);
   void startDecode();
  protected:
   void pushFrame();
   void run();
signals:
    void onNewFrame();
 public:
    Frames * m_frames;
 protected:
    const char* filePath;

};

#endif // VTDECODER_H
