#include "vtdecoder.h"
#include <QDebug>
#include <QImage>
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

void logErrorMsg(const char* tag,int ret){
    char errorbuf[255] = { 0 };
    av_strerror(ret, errorbuf, 254);
    qCritical("%s: retcode:%d,errorMsg: %s",tag,ret,errorbuf);
}

VTDecoder::VTDecoder()
{
}
void VTDecoder::startDecode(){
    this->start();
}
void VTDecoder::run(){

    AVFormatContext* formatContext = avformat_alloc_context();
    //设置mp4文件路径
    int res = avformat_open_input(&formatContext,filePath,nullptr,nullptr);
    if(res == 0){
        qDebug()<<"open file success";
    }else{
        qDebug()<<"open file failed";
    }

    res = avformat_find_stream_info(formatContext,nullptr);
    if(res >= 0){
        qDebug()<< "find video info success";
    }else {
        qDebug()<< "find vido info failed";
    }

    int videoType = -1;
    for(unsigned int i = 0 ; i <formatContext->nb_streams;i++){
        AVStream* stream = formatContext->streams[i];
        //查找视频流
        if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            videoType = i;
            break;
        }
    }

    if(videoType == -1){
        qDebug()<< "没有视频流";
        return;
    }else{
        qDebug()<<"有视频流:"<<videoType;
    }

    AVStream* stream = formatContext->streams[videoType];
    AVCodecParameters* codecParameter = stream->codecpar;

    //解码器
    const AVCodec* decoder = avcodec_find_decoder(codecParameter->codec_id);

    if(decoder != nullptr){
        qDebug()<<"找到解码器:"<<decoder->name;
    }else{
        qDebug()<<"没找到解码器";
        return;
    }

    //申请CodecContext
    AVCodecContext* codec = avcodec_alloc_context3(decoder);
    //少了这句话
    if(avcodec_parameters_to_context(codec, stream->codecpar)<0){
        qDebug()<<"avcodec_parameters_to_context failed";
        return ;
    }
    //打开解码器
    res = avcodec_open2(codec,decoder,nullptr);

    //打开编码器失败
    if(res !=0){
        qDebug()<< "open codec failed";
        return;
    }
    //宽高
    int width =codecParameter->width;
    int height = codecParameter->height;

    //比特率
    int bit_reate = codecParameter->bit_rate;
    //0:视频 1:音频  2:数据 3:标题
    int codec_type = codecParameter->codec_type;
    //视频的像素格式 或者 音频的采样格式
    int formate = codecParameter->format;
    //视频延迟帧数
    int video_delay = codecParameter->video_delay;

    av_dump_format(formatContext, 0, filePath, false);
    qDebug()<<"width:"<<width<<" height:"<<height<<" bitRate:"<<bit_reate<<" codecType:"<<codec_type <<" format:"<<formate<<" videoDelay:"<<video_delay;

    AVRational sample_aspect_ratio = codecParameter->sample_aspect_ratio;
    qDebug()<<"sample_aspect_ratio.num:"<<sample_aspect_ratio.num<<" sample_aspect_ratio.den:"<<sample_aspect_ratio.den<<"";
    qDebug()<<"avg_frame_rate.num:"<<stream->avg_frame_rate.num<<" avg_frame_rate.den:"<<stream->avg_frame_rate.den;

    //计算fps 帧率
    float frameRate = stream->avg_frame_rate.num/stream->avg_frame_rate.den;
    qDebug()<<"frameRate:"<<frameRate;

    //申请AVPacket
    AVPacket* packet = av_packet_alloc();
    packet->data = NULL;
    packet->size = 0;

    if(m_frames == nullptr){
        qDebug()<<"m_frames is null";
        return ;
    }


    while(!av_read_frame(formatContext,packet)){

        if (packet->stream_index == videoType) {

            int ret = avcodec_send_packet(codec,packet);
            if(ret < 0||ret == AVERROR(EAGAIN) || ret == AVERROR_EOF){
                logErrorMsg("avcodec_send_packet error:", ret);
                goto runQuit;
                break;
            }

            AVFrame* decodingFrame = m_frames->decodingFrame();
            if (decodingFrame) {
                ret = avcodec_receive_frame(codec, decodingFrame);
                if(!ret){
//                    qDebug()<<"avframe after dcode:"<<decodingFrame->width<<"x"<<decodingFrame->height;
                    pushFrame();

                }else{
                    av_packet_unref(packet);
                    logErrorMsg("avcodec_receive_frame other error:", ret);
                    goto runQuit;
                }

                //解码后的数据
                // decodingFrame->data;
//                qDebug()<<"frame:"<<codec->frame_number;
                //延时
                usleep(1000*1000/frameRate);
                av_packet_unref(packet);

            }

        }

    }

runQuit:
    qDebug()<<"quiting";
    if (formatContext) {
        avformat_close_input(&formatContext);
    }
    if (formatContext) {
        avformat_free_context(formatContext);
    }
    if (codec ) {
        avcodec_close(codec);
    }
    if (codec) {
        avcodec_free_context(&codec);
    }

}
void VTDecoder::setFrames(Frames* frames){
    this->m_frames = frames;
}

bool VTDecoder::init(){

#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 9, 100)
    av_register_all();
#endif
    if (avformat_network_init()) {
        return false;
    }
    return true;


}
void VTDecoder::setFilePath(const char* filePath){
    this->filePath = filePath;
}

void VTDecoder::pushFrame()
{
    bool previousFrameConsumed = m_frames->offerDecodedFrame();
    if (!previousFrameConsumed) {
        // the previous newFrame will consume this frame
        return;
    }
    qDebug()<<"push frame";
    emit onNewFrame();
}
