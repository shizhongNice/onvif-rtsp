#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

#include "xcam_rtsp.h"
#include "xcam_onvif.h"
#include "xcam_stream.h"
#include "xcam_log.h"

#include "shareMem.h"

#define LOG_TAG "MAIN"

char* start_str =                  \
    "\n __  __                      \n"\
    " \\ \\/ /___ __ _ _ __ ___     \n"\
    "  \\  // __/ _` | '_ ` _ \\    \n"\
    "  /  \\ (_| (_| | | | | | |    \n"\
    " /_/\\_\\___\\__,_|_| |_| |_|  \n\n";


int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    printf("%s", start_str);
    fflush(stdout);

    rtsp_init();

    onvif_init();

    sharemem_init();

    stream_create(0);

    while (1)
    {
        sleep(1);
    }

    return 0;
}
