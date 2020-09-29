#ifndef _SYS_THREAD_H_
#define _SYS_THREAD_H_

int xcam_thread_create(char * name, void *(*start_routine) (void *), void *param);
int xcam_thread_set_name(char * name);

#endif
