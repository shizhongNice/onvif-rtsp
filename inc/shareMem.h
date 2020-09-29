/*
 * shareMem.h
 *
 *  Created on: Feb 26, 2019
 *      Author: XiaoYJ
 */

#ifndef _SHAREMEM_H_
#define _SHAREMEM_H_

#include <pthread.h>
#include <sys/time.h>
#include "dataassist.h"

#define SHARE_MEM_VIDEO_NM			4
#define SHARE_MEM_AUDIO_NM			5
#define SHARE_MEM_AUDIO_RECEIVE_NM  10

typedef struct AUDIO_FRAME_REV_INFO
{
	unsigned char *pBuf;		///< frame address
	int chs;					///< channel number
	int nlen;					///< frame len
	int samplerate;				///< Sampling rate
	long long tim;				///< timestamp
}a_frameRev_info_s;

typedef struct _mem_stream_audioRev_info
{
	a_frameRev_info_s audioRev_st;
	unsigned char buf[10*1024];
}a_audioRev_frame_s;

typedef struct _mem_stream_videoSnd_info
{
	v_frame_info_s videoSnd_st;
	unsigned char buf[256*1024];
}a_videoSnd_frame_s;

typedef struct _mem_stream_audioSnd_info
{
	a_frame_info_s audioSnd_st;
	unsigned char buf[1*1024];
}a_audioSnd_frame_s;

typedef struct _mem_share_video_info_list
{
	unsigned int add_video_Num;
	unsigned int get_video_Num;
	a_videoSnd_frame_s videoSnd_data[SHARE_MEM_VIDEO_NM];//发送视频帧
}video_info_list_t;

typedef struct _mem_share_audio_info_list
{
	unsigned int add_audio_Num;
	unsigned int get_audio_Num;
	a_audioSnd_frame_s audioSnd_data[SHARE_MEM_AUDIO_NM];//发送音频帧
}audio_info_list_t;

typedef struct _mem_share_speak_info_list
{
	unsigned int add_speak_Num;
	unsigned int get_speak_Num;
	a_audioRev_frame_s audioRev_data[SHARE_MEM_AUDIO_RECEIVE_NM];//接收音频帧
}speak_info_list_t;

typedef struct _tag_stream_snd_ctrl
{
	void *videoShmAddr;
	void *audioShmAddr;
	void *speakShmAddr;
}stream_snd_ctrl_s;

typedef struct _tag_shm_head_
{
	pthread_mutex_t lock;
}shm_head_s;

stream_snd_ctrl_s *get_stream_ctrl(void);
int sharemem_init(void);

#endif