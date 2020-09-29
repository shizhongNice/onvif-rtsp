#ifndef _VIDEO_SOURCE_H_
#define _VIDEO_SOURCE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

typedef struct sourceinfo {
    int channel_num;
    int (*cb_oncreate)(int chnum, void *object);
    int (*cb_ondestroy)(int chnum, void *object);
    int (*cb_getframe)(int chnum);
} sourceinfo_t;
int video_source_init(void);
int video_source_info_init(sourceinfo_t* info, int chnum);
int video_source_isready(int chnum);
int video_source_waitready(int chnum);
int video_source_putframe(int chnum, unsigned char* buf, int size, int64_t ts);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
