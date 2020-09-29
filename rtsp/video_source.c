#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#include "video_source.h"
#include "c_liveRTSP.h"

#include "xcam_log.h"
#define LOG_TAG "RTSP"

typedef struct sourcectrl {
    void *object;
    int valid;
    int isready;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sourcectrl_t;

#define SOURCE_NUM 6

sourcectrl_t sctrl[SOURCE_NUM];

int video_source_init(void)
{
    int i = 0;
    memset(sctrl, 0, sizeof(sctrl));
    for (i = 0; i < SOURCE_NUM; i++) {
        pthread_mutex_init(&sctrl[i].mutex,NULL);
        pthread_cond_init(&sctrl[i].cond,NULL);
    }
    return 0;
}

int video_source_cb_oncreate(int chnum, void *object)
{
    LOG_DBG(LOG_TAG, "num = %d\n", chnum);
    sctrl[chnum].valid = 1;
    sctrl[chnum].object = object;
    return 0;
}

int video_source_cb_oncdestroy(int chnum, void *object)
{
    LOG_DBG(LOG_TAG, "num = %d\n", chnum);
    pthread_mutex_lock(&sctrl[chnum].mutex);
    sctrl[chnum].valid = 0;
    sctrl[chnum].isready = 0;
    sctrl[chnum].object = NULL;
    pthread_mutex_unlock(&sctrl[chnum].mutex);
    return 0;
}

int video_source_cb_getframe(int chnum)
{
    //LOG_DBG(LOG_TAG, "get frame num = %d\n", chnum);
    pthread_mutex_lock(&sctrl[chnum].mutex);
    sctrl[chnum].isready = 1;
    pthread_cond_signal(&sctrl[chnum].cond);
    pthread_mutex_unlock(&sctrl[chnum].mutex);
    return 0;
}

int video_source_info_init(sourceinfo_t* info, int chnum)
{
    info->channel_num = chnum;
    info->cb_oncreate = video_source_cb_oncreate;
    info->cb_ondestroy = video_source_cb_oncdestroy;
    info->cb_getframe = video_source_cb_getframe;
    return 0;
}

int video_source_isready(int chnum)
{
    return sctrl[chnum].isready;
}

int video_source_waitready(int chnum)
{
    struct timeval now;
    struct timespec timeout;

    pthread_mutex_lock(&sctrl[chnum].mutex);

    while (0 == sctrl[chnum].isready) {
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + 5;
        timeout.tv_nsec = now.tv_usec * 1000;
        if(pthread_cond_timedwait(&sctrl[chnum].cond, &sctrl[chnum].mutex, &timeout) == ETIMEDOUT) {
            LOG_WAN(LOG_TAG, "wait timeout chnum = %d\n", chnum);
        }
    }
    pthread_mutex_unlock(&sctrl[chnum].mutex);

    return 0;
}

int video_source_putframe(int chnum, unsigned char* buf, int size, int64_t ts)
{
    //LOG_DBG(LOG_TAG, "put frame num = %d\n", chnum);
    pthread_mutex_lock(&sctrl[chnum].mutex);
    if (1 == sctrl[chnum].valid)
        c_RTSP_put_source_frame(sctrl[chnum].object, buf, size, ts);
    sctrl[chnum].isready = 0;
    pthread_mutex_unlock(&sctrl[chnum].mutex);
    return 0;
}
