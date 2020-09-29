/*
 * shareMem.c
 *
 *  Created on: Feb 26, 2019
 *      Author: XiaoYJ
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "shareMem.h"

stream_snd_ctrl_s g_streamCtrl;

stream_snd_ctrl_s *get_stream_ctrl(void)
{
	return &g_streamCtrl;
}

void *get_share_Memory(const int MemoryID, int MemorySize)
{
    int shmid = -1;
	void *shmaddr = NULL;

    shmid = shmget(MemoryID, MemorySize, (0666|IPC_CREAT));
    if(shmid == -1)
    {
		perror("shmget create error") ;
        return NULL;
    }

	shmaddr = shmat(shmid, NULL, 0);
	if (shmaddr == (void *)-1)
	{
		perror("shmat addr error") ;
		return NULL;
	}

    return shmaddr;
}

int sharemem_init(void)
{
	stream_snd_ctrl_s *ctx = get_stream_ctrl();

	//初始化视频共享内存
	ctx->videoShmAddr = (video_info_list_t *)get_share_Memory(60001,sizeof(shm_head_s) + sizeof(video_info_list_t));
	if(ctx->videoShmAddr == NULL)
	{
		return -1;
	}

	//初始化音频共享内存
	ctx->audioShmAddr = (audio_info_list_t *)get_share_Memory(60002,sizeof(shm_head_s) + sizeof(audio_info_list_t));
	if(ctx->audioShmAddr == NULL)
	{
		return -1;
	}

	//初始化speak共享内存
	ctx->speakShmAddr = (speak_info_list_t *)get_share_Memory(60003,sizeof(shm_head_s) + sizeof(speak_info_list_t));
	if(ctx->speakShmAddr == NULL)
	{
		return -1;
	}

	printf("create ok, ready to start \n");
    fflush(stdout);

	return 0;
}
