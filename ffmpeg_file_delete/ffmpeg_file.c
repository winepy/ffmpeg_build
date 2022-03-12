
// pkg-config --libs libavformat 查找libavformat 这个库的位置

// clang -g -o ffmpeg_del ffmpeg_file.c -llibavformat

// clang -g -o ffmpeg_del ffmpeg_file.c `pkg-config --libs libavformat`
// ./ffmpeg_del
#include <libavformat/avformat.h>

int main(){
    int ret;
    ret = avpriv_io_move("111.txt","222.txt");
    if(ret<0){
        av_log(NULL, AV_LOG_ERROR, "fail to rename file 111.txt\n");
    }
    av_log(NULL, AV_LOG_ERROR, "success to rename file 111.txt\n");

    ret = avpriv_io_delete("./mytextfile.txt");
    if(ret<0){
        av_log(NULL, AV_LOG_ERROR, "fail to delete file mytextfile.txt\n");
        return -1;
    }
    av_log(NULL, AV_LOG_ERROR, "success to delete file mytextfile.txt\n");


    return 0;
}