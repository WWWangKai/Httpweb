#include"httpd.h"
#include"threadpool.h"

int count = 0;
static void usage(const char* proc)
{
	printf("USAGE: %s [local_ip][local_port]\n",proc);
}

void *func(void *arg)
{
	int sock = (int )arg;
	return (void*)handle_request(sock);
	return NULL;
}


int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(argv[1],atoi(argv[2]));
	if (tpool_create(8) != 0) 
	{
		printf("tpool_create failed\n");  
		exit(1);  
	}  
	while(1)
	{
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_sock < 0)
		{
			perror("accept");
			continue;
		}
	printf("get a new client!\n");
	printf("count = %d\n",count++);
	if(tpool_add_work(func,(void*)new_sock) != 0)
	{
		printf("tpool_add_work is faild\n");
		close(new_sock);
	}
    //pthread_t id;
	//pthread_create(&id,NULL,&handle_request,(void*)new_sock);
	//pthread_detach(id);
	}
	close(listen_sock);
	tpool_destroy();
	return 0;
}









