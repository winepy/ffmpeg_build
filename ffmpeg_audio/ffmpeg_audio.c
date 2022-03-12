#include <stdio.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>

void adts_header(char *szAdtsHeader, int dataLen){

    int audio_object_type = 2;
    int sampling_frequency_index = 7;
    int channel_config = 2;

    int adtsLen = dataLen + 7;

    szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
    szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
    szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
    szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits 
    szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

    szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
    szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits 
    szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
    szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

    szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
    szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
    szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
    szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit  
    szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
    szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

    szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
    szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
    szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
    szAdtsHeader[6] = 0xfc;
}

int main(){
    AVFormatContext *fmt_ctx = NULL;
    av_log_set_level(AV_LOG_DEBUG);

    int ret ;
    avformat_network_init();//av_register_all()过期了

    ret = avformat_open_input(&fmt_ctx, "./test.mp4",NULL,NULL);

    if(ret<0){
        av_log(NULL,AV_LOG_ERROR,"cannt open file %s \n",av_err2str(ret));
        return -1;
    }
    FILE* dst_file = fopen("out.aac","wb");
    if(!dst_file){
        av_log(NULL ,AV_LOG_ERROR,"cannot open out file");
        avformat_close_input(&fmt_ctx);
        return -1;
    }
    av_dump_format(fmt_ctx,0,"./test.mp4",0);//打印meta信息

    //只处理音频，其他的先不用管,返回音频流的index
    ret = av_find_best_stream(fmt_ctx,AVMEDIA_TYPE_AUDIO,-1,-1,NULL,0);
    if(ret<0){
        av_log(NULL ,AV_LOG_ERROR,"cannot find best stream");
        avformat_close_input(&fmt_ctx);
        fclose(dst_file);
        return -1;
    }
    int audio_index;
    audio_index = ret;
    AVPacket pkt;
    av_init_packet(&pkt);
    while(av_read_frame(fmt_ctx, &pkt)>=0){
        if(pkt.stream_index == audio_index){

            //写 aac的头部 
            char adts_header_buf[7];
            adts_header(adts_header_buf ,pkt.size);
            fwrite(adts_header_buf,1,7,dst_file);
            int len = fwrite(pkt.data,1,pkt.size,dst_file);
            if(len!=pkt.size){
                av_log(NULL,AV_LOG_ERROR,"write out file error");
            }
        }
        av_packet_unref(&pkt);
    }

    avformat_close_input(&fmt_ctx);

    if(dst_file){
        fclose(dst_file);
    }

    return 0;
}