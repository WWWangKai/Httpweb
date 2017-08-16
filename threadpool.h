#include <pthread.h>   
#include <unistd.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <stdio.h> 

typedef struct tpool_work {  
	void*	(*routine)(void*);       /* 任务函数 */  
	void    *arg;                    /* 传入任务函数的参数 */  
	struct tpool_work   *next;                      
}tpool_work_t;  

typedef struct tpool {  
	int             shutdown;                     
	int             max_thr_num;            
	pthread_t       *thr_id;                
	tpool_work_t    *queue_head;           
	pthread_mutex_t queue_lock;                      
	pthread_cond_t  queue_ready;      
}tpool_t; 

int tpool_create(int max_thr_num);  
void tpool_destroy();  
int tpool_add_work(void*(*routine)(void*), void *arg);


