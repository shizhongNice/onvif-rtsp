#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <errno.h>

#define THREAD_NAME_MAX_LEN (20)
#define THREAD_NUM_MAX (50)
typedef struct thread_info {
    char valid;
    char name[THREAD_NAME_MAX_LEN];
    pthread_t handle_thread;
} thread_info_t;

thread_info_t threads_info[THREAD_NUM_MAX];

int xcam_thread_create(char * name, void *(*start_routine) (void *), void *param)
{
    int ret = 0;
    int tid = -1;
    int i = 0;
    if ((THREAD_NAME_MAX_LEN-1) < strlen(name)) {
        printf("error(%s,%d): thread name too lenght\n", __func__, __LINE__);
        fflush(stdout);
        return -1;
    }
    for (i = 0; i < THREAD_NUM_MAX; i++) {
        if (0 == threads_info[i].valid) {
            tid = i;
            break;
        }
    }
    if (-1 == tid) {
        printf("error(%s,%d): no empty thread info\n", __func__, __LINE__);
        fflush(stdout);
        return -1;
    }
    ret =  pthread_create(&threads_info[tid].handle_thread, NULL, start_routine, param);
   if (0 != ret) {
        printf("error(%s,%d): thread create failed %s\n", __func__, __LINE__, strerror(errno));
        fflush(stdout);
        return -1;
    }
    strcpy(threads_info[tid].name, name);

    return tid;
}

int xcam_thread_set_name(char * name)
{
    prctl(PR_SET_NAME, name, 0, 0, 0);
    return 0;
}
