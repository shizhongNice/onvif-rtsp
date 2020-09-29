#ifndef _C_LIVE_RTSP_H
#define _C_LIVE_RTSP_H

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

int c_RTSP_start();
int c_RTSP_put_source_frame(void* object, unsigned char* buf, int size, int64_t ts);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
