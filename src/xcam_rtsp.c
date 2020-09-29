#include <stdio.h>
//#include "xcam_com.h"
//#include "xcam_module.h"

#include "xcam_log.h"
#include "xcam_stream.h"
#include "xcam_rtsp.h"
#include "rtsp/video_source.h"
#include "sys/time.h"
#include "xcam_thread.h"
#include "rtsp/c_liveRTSP.h"

void* thread_RTSP(void * param)
{
    xcam_thread_set_name("rtsp_main");
    c_RTSP_start();
    return NULL;
}

int rtsp_init(void)
{
    xcam_thread_create("rtsp_main", thread_RTSP, NULL);
    return 0;
}
