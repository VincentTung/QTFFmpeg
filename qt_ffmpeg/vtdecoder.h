#ifndef VTDECODER_H
#define VTDECODER_H

#include <frames.h>
class VTDecoder
{
public:
    VTDecoder();
   bool init();
   void openFile(const char* filePath);
   void setFrames(Frames * frames);
 public:
    Frames * m_frames;
};

#endif // VTDECODER_H
