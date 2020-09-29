#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>

#include "xcam_log.h"
#include "xcam_thread.h"
#include "xcam_stream.h"
#include "rtsp/video_source.h"

#include "shareMem.h"

#define LOG_TAG "STREAM"

#define FRAMEBUFSIZE (2*1024*1024)
#define BUFSIZE 8192

//#define DEMO_TEST

#define STREAM_OUTPUT


int stream_from_sharemem(int channel);

int stream_from_h264file(int channel);


void* thread_stream(void * param)
{
    int channel = *(int *)param;
    static int rtsp_init_flag = 0;

    if (0 == rtsp_init_flag)
    {
        sleep(1);
        video_source_init();
        rtsp_init_flag = 1;
    }

    if(channel != 0 && channel != 1)
    {
        LOG_ERR(LOG_TAG, "channel param error! (%d)\n", channel);
        return NULL;
    }

    while(1)
    {
#ifdef DEMO_TEST
        stream_from_h264file(channel);
#else
        stream_from_sharemem(channel);
#endif
    }

    return NULL;
}

int stream_create(int channel)
{
    char threadname[20];
    sprintf(threadname, "stream-%d", channel);
    xcam_thread_create(threadname, thread_stream, &channel);
    return 0;
}


int stream_from_sharemem(int channel)
{
    LOG_DBG(LOG_TAG, "video_Thread begin \n");
    int index = -1;
    u_int32_t framelen = 0;
    u_int64_t timestamp =  0;
    int       frameType = 0;
    u_int32_t frameSeq = 0;
    unsigned char *framebuf = NULL;

    stream_snd_ctrl_s *pst = get_stream_ctrl();
    shm_head_s *head = (shm_head_s *)pst->videoShmAddr;
    video_info_list_t *ctrlInfo = (video_info_list_t *)(head+1);

    framebuf = (unsigned char *)malloc(FRAMEBUFSIZE);
    if (NULL == framebuf) {
        LOG_ERR(LOG_TAG, "malloc failed\n");
        return -1;
    }

#ifdef STREAM_OUTPUT
    char *h264output = "/system/output.h264";
    int fd_out = -1;
    fd_out = open(h264output, O_RDWR|O_CREAT|O_TRUNC );
    if (fd_out < 0) {
        LOG_ERR(LOG_TAG, "open h264file failed\n");
        return -1;
    }
#endif

    while(1)
    {
        LOG_DBG(LOG_TAG, ".............\n");
        pthread_mutex_lock(&(head->lock));

        int tmpNum = ctrlInfo->get_video_Num % SHARE_MEM_VIDEO_NM;
        if((ctrlInfo->add_video_Num != ctrlInfo->get_video_Num) &&
                (ctrlInfo->videoSnd_data[tmpNum].videoSnd_st.len != 0))
        {
            index = tmpNum;
            framelen = ctrlInfo->videoSnd_data[index].videoSnd_st.len;
            timestamp = ctrlInfo->videoSnd_data[index].videoSnd_st.timestamp.tv_sec * 1000 * 1000
                    + ctrlInfo->videoSnd_data[index].videoSnd_st.timestamp.tv_usec;

            frameType =  ctrlInfo->videoSnd_data[index].videoSnd_st.frameType;
            frameSeq = ctrlInfo->videoSnd_data[index].videoSnd_st.frameSeq;
            if(framelen <= FRAMEBUFSIZE)
            {
                memcpy(framebuf, ctrlInfo->videoSnd_data[index].buf, framelen);

#ifdef STREAM_OUTPUT
                int retcnt = 0;
                retcnt = write(fd_out, framebuf, framelen);
                if (retcnt != framelen) {
                    LOG_ERR(LOG_TAG, "write error ret = %d, write = %d\n", retcnt, framelen);
                }
#endif
            }
        }
        else
        {
            index = -1;
            if((ctrlInfo->add_video_Num - ctrlInfo->get_video_Num) >= SHARE_MEM_VIDEO_NM)
            {
                ctrlInfo->get_video_Num = ctrlInfo->add_video_Num;
                LOG_DBG(LOG_TAG, ".............\n");
            }
        }

        if(index != -1)
        {
            ctrlInfo->videoSnd_data[index].videoSnd_st.len = 0;
            ctrlInfo->get_video_Num++;
            LOG_DBG(LOG_TAG, "get a send video num is %d \n", ctrlInfo->get_video_Num);
        }
        pthread_mutex_unlock(&(head->lock));
        usleep(40 * 1000);

        LOG_DBG(LOG_TAG, " video %d: framelen: %d frameType: %d frameSeq: %d timestamp: %u\n",
                index, framelen, frameType, frameSeq, timestamp);

        int isready = video_source_isready(channel);
        LOG_DBG(LOG_TAG, "isready: %d, data:[%02x][%02x][%02x][%02x][%02x]\n",
                isready, framebuf[0], framebuf[1], framebuf[2], framebuf[3], framebuf[4]);
        //if(isready)
        {
            video_source_waitready(channel);
            video_source_putframe(channel, framebuf+4, framelen-4, timestamp);
        }
    }
#ifdef STREAM_OUTPUT
    close(fd_out);
#endif
    free(framebuf);

    return 0;
}

int stream_from_h264file(int channel)
{
    char *h264input = "/system/video_main.h264";

    int fd_bs = -1;
    unsigned char *bsbuf = NULL;
    unsigned char *framebuf = NULL;
    int firstread = 0;
    int start = 0;
    unsigned char *pfbuf;
    int i = 0;

    bsbuf = (unsigned char *)malloc(BUFSIZE);
    if (NULL == bsbuf) {
        LOG_ERR(LOG_TAG, "malloc failed\n");
        return -1;
    }
    framebuf = (unsigned char *)malloc(FRAMEBUFSIZE);
    if (NULL == framebuf) {
        LOG_ERR(LOG_TAG, "malloc failed\n");
        return -1;
    }

    fd_bs = open(h264input, O_RDONLY);
    if (fd_bs < 0) {
        LOG_ERR(LOG_TAG, "open %s failed\n", h264input);
        return -1;
    }

    struct timeval ts;
    int64_t timestamp;

    firstread = 0;
    start = 0;
    pfbuf = framebuf;

    while (1)
    {
        int readcnt = 0;
        unsigned char* pbuf = bsbuf;
        unsigned char* pstart = bsbuf;
        if (0 == firstread) {
            readcnt = read(fd_bs, bsbuf, BUFSIZE);
            firstread = 1;
        } else {
            readcnt = read(fd_bs, bsbuf+4, BUFSIZE-4);
        }
        if (readcnt <= 0) {
            //LOG_DBG(LOG_TAG, "readcnt = %d\n", readcnt);
            break;
        }
        usleep(20);

        int cnt = BUFSIZE-4;
        int last = 0;
        if (readcnt < BUFSIZE-4) {
            cnt = readcnt;
            last = 1;
        }
        for (i = 0; i < cnt; i++)
        {
            if ((pbuf[0] == 0x00) && (pbuf[1] == 0x00) &&
                    ((pbuf[2] == 0x01) || ((pbuf[2] == 0x00) && (pbuf[3] == 0x01))))
            {
                if (0 == start) {
                    start = 1;
                } else {
                    memcpy(pfbuf, pstart, pbuf-pstart);
                    pfbuf += (pbuf-pstart);
                    start = 0;

                    //get frame
                    video_source_waitready(channel);
                    //LOG_DBG(LOG_TAG, "wait RTSP ready ... ");
                    usleep(40*1000);
                    gettimeofday(&ts, NULL);
                    timestamp = ts.tv_sec*1000000+ts.tv_usec;

                    int framelen = pfbuf-framebuf;

                    video_source_putframe(channel, framebuf, framelen, timestamp);

                    pfbuf = framebuf;
                    start = 1;

                }
                if (pbuf[2] == 0x01)
                    pbuf+=3;
                else
                    pbuf+=4;
                pstart = pbuf;
            } else {
                pbuf++;
            }
        }
        if (1 == last) {
            //get last frame
            break;
        }

        if (1 == start) {
            memcpy(pfbuf, pstart, bsbuf+BUFSIZE-pstart-4);
            pfbuf += (bsbuf+BUFSIZE-pstart-4);
        }
        memmove(bsbuf, bsbuf+BUFSIZE-4, 4);
    }
    close(fd_bs);
    free(bsbuf);
    free(framebuf);

    return 0;
}
