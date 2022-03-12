// clang -g -o mediainfo ffmpeg_media_info.c `pkg-config --libs libavutil libavformat`
// ./mediainfo
#include <stdio.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>

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

    av_dump_format(fmt_ctx,0,"./test.mp4",0);//打印meta信息

    avformat_close_input(&fmt_ctx);


    return 0;
}