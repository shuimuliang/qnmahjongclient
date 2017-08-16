#include "semaphore.h"
#include "CCPThreadWinRT.h"

int* p_semlock;
int p_semint;
 int __cdecl sem_init (sem_t * sem,
                int pshared,
                unsigned int value){
 p_semint = 0;
 p_semlock = &p_semint;
 return 1;
 }

 int __cdecl sem_destroy (sem_t * sem){return 1;}

 int __cdecl sem_trywait (sem_t * sem){
	 p_semlock = &p_semint;
	 *p_semlock = 1;
	 while (*p_semlock!=0)
	 {
		 Concurrency::wait(16);	
	 }
	 return 1;
 }

 int __cdecl sem_wait (sem_t * sem){
	 p_semlock = &p_semint;
	 *p_semlock = 1;
	 while (*p_semlock!=0)
	 {
		 Concurrency::wait(16);	
	 }
	 return 1;
 }

 int __cdecl sem_timedwait (sem_t * sem,
                 const struct timespec * abstime){return 1;}

 int __cdecl sem_post (sem_t * sem){
	 *p_semlock = 0;
	 return 1;
 }

 int __cdecl sem_post_multiple (sem_t * sem,
                     int count){return 1;}

 int __cdecl sem_open (const char * name,
                int oflag,
                mode_t mode,
                unsigned int value){return 1;}

 int __cdecl sem_close (sem_t * sem){return 1;}

 int __cdecl sem_unlink (const char * name){return 1;}

 int __cdecl sem_getvalue (sem_t * sem,
                int * sval){return 1;}
