#ifndef VTDECODER_H
#define VTDECODER_H

#include <frames.h>
class VTDecoder:public QObject
{
 Q_OBJECT
public:
    VTDecoder();
   bool init();
   void openFile(const char* filePath);
   void setFrames(Frames * frames);
   void pushFrame();
signals:
    void onNewFrame();
 public:
    Frames * m_frames;
};

#endif // VTDECODER_H
