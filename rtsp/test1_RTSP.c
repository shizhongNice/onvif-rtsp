#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "c_liveRTSP.h"
#include "video_source.h"


#define FRAMEBUFSIZE (2*1024*1024)
#define BUFSIZE 8192
//#define FRAME_OUT


void* thread_RTSP(void * param)
{
    c_RTSP_start();
    return NULL;
}



int main(int argc, char** argv)
{
	pthread_t tid_RTSP;
	if (argc < 2) {
		printf("Usage:%s h264bsfile", argv[0]);
		return -1;
	}

	pthread_create(&tid_RTSP, NULL, thread_RTSP, NULL);
    //c_RTSP_start();
    usleep(2*1000*1000);

loop:
    {
        int fd_bs = -1;
#ifdef FRAME_OUT
        int fd_out = -1;
#endif
        unsigned char *bsbuf = NULL;
        unsigned char *framebuf = NULL;
        int firstread = 0;
        int start = 0;
        unsigned char *pfbuf;
        int i = 0;

        bsbuf = malloc(BUFSIZE);
        if (NULL == bsbuf) {
            printf("malloc failed\n");
            return -1;
        }
        framebuf = malloc(FRAMEBUFSIZE);
        if (NULL == framebuf) {
            printf("malloc failed\n");
            return -1;
        }

        fd_bs = open(argv[1], O_RDONLY);
        if (fd_bs < 0) {
            printf("open %s failed\n", argv[1]);
            return -1;
        }

#ifdef FRAME_OUT
        fd_out = open("out.h264", O_RDWR|O_CREAT);
        if (fd_out < 0) {
            printf("open %s failed %s\n", "out.h264", strerror(errno));
            return -1;
        }
#endif
        struct timeval ts;
        int64_t timestamp;

        firstread = 0;
        start = 0;
        pfbuf = framebuf;
        while (1) {
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
                //printf("readcnt = %d\n", readcnt);
                break;
            }
            usleep(20);

            int cnt = BUFSIZE-4;
            int last = 0;
            if (readcnt < BUFSIZE-4) {
                cnt = readcnt;
                last = 1;
            }
            for (i = 0; i < cnt; i++) {
                if ((pbuf[0] == 0x00) && (pbuf[1] == 0x00) &&
                        ((pbuf[2] == 0x01) || ((pbuf[2] == 0x00) && (pbuf[3] == 0x01)))) {
                    if (0 == start) {
                        start = 1;
                    } else {
                        memcpy(pfbuf, pstart, pbuf-pstart);
                        pfbuf += (pbuf-pstart);
                        start = 0;

                        //get frame
                        video_source_waitready(0);
                        //printf("wait RTSP ready ... ");
                        usleep(40*1000);
                        gettimeofday(&ts, NULL);
                        timestamp = ts.tv_sec*1000000+ts.tv_usec;

                        video_source_putframe(0, framebuf, pfbuf-framebuf, timestamp);
                        //printf("put frame size = %d\n", pfbuf-framebuf);
                        //
                        {
#if 0
                            int i = 0;
                            printf("size = %-8d :", pfbuf-framebuf);
                            for (i = 0; i < 16; i++) printf("%02x ", framebuf[i]);
                            printf("\n");
                            //usleep(1*1000*1000);
#endif
#ifdef FRAME_OUT
                            int retcnt = 0;
                            retcnt = write(fd_out, framebuf, pfbuf-framebuf);
                            if (retcnt != pfbuf-framebuf) {
                                printf("write error ret = %d, write = %d\n", retcnt, pfbuf-framebuf);
                            }
#endif

                        }

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
#if 0
                int retcnt = 0;
                retcnt = write(fd_out, pstart, bsbuf+cnt-pstart+4);
                if (retcnt != bsbuf+cnt-pstart+4) {
                    printf("write error ret = %d, write = %d\n", retcnt, bsbuf+cnt-pstart+4);
                }
#endif
                break;
            }

            if (1 == start) {
                memcpy(pfbuf, pstart, bsbuf+BUFSIZE-pstart-4);
                pfbuf += (bsbuf+BUFSIZE-pstart-4);
            }
            memmove(bsbuf, bsbuf+BUFSIZE-4, 4);
        }
        close(fd_bs);
#ifdef FRAME_OUT
        close(fd_out);
#endif
        free(bsbuf);
        free(framebuf);

    }
    goto loop;
    return 0;
}
