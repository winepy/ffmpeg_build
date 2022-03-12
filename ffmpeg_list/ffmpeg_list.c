
//  clang -g -o ffmepg_list ffmpeg_list.c `pkg-config --libs libavformat libavutil`
// ./ffmepg_list
//

#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(){
    int ret;
    av_log_set_level(AV_LOG_DEBUG);
    AVIODirContext *ctx = NULL;//文件上下文
    AVIODirEntry *entry=NULL;
    ret = avio_open_dir(&ctx, "./" , NULL);
    if(ret < 0){
        av_log(NULL,AV_LOG_ERROR, "ccant open dir %s \n",av_err2str(ret));
        return -1;
    }

    while(1){
        avio_read_dir(ctx, &entry);
        if(ret<0){
            av_log(NULL, AV_LOG_ERROR, "cannt read dir %s \n",av_err2str(ret));
            goto __fail;
        }
        if(!entry){//entry == null:目录最后一个
            break;
        }
        av_log( NULL, AV_LOG_INFO, "%12"PRId64"    %s \n",entry->size,entry->name);
        //释放entry
        avio_free_directory_entry(&entry);

    }
__fail:
    avio_close_dir(&ctx);

    return 0;

}