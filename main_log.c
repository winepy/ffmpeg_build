// 命令行输入下面
// clang -g -o main_log main_log.c -lavutil
// main_log

#include <stdio.h>
#include <libavutil/log.h>

int main(){
    av_log_set_level(AV_LOG_DEBUG);
    av_log(NULL, AV_LOG_INFO, "HELLO world!! \n");
    return 0;
}