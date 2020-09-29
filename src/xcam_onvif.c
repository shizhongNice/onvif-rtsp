#include <stdio.h>
#include "xcam_log.h"
#include "xcam_stream.h"
#include "xcam_thread.h"
#include "onvif.h"

int onvif_init(void)
{
    xcam_thread_create("onvif_main", onvif_start, NULL);
    return 0;
}
