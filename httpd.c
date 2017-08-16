#include"httpd.h"


int startup(char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}


	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}
	
	if(listen(sock,10) < 0)
	{
		perror("listen");
		exit(4);
	}
	return sock;
}

void clear_header(int sock)
{
	printf("clear_header\n");
	int ret = 0;
	char buf[SIZE];
	do{
		ret = get_line(sock,buf,sizeof(buf));
		printf("%s",buf);
	}while(ret > 0 && strcmp(buf,"\n") != 0);
}


void echo_client(int nums)
{
}

void echo_www(int sock,char *path,int size)
{
	printf("echo_www\n");
	printf("path%s\n ",path);
	int new_fd = open(path,O_RDONLY);
	if(new_fd < 0)
	{
		return;
	}
	const char *status_line = "HTTP/1.0 200 OK\r\n";
	const char *blank_line = "\r\n";
	send(sock,status_line,strlen(status_line),0);
	send(sock,blank_line,strlen(blank_line),0);
	sendfile(sock,new_fd,NULL,size);
	close(new_fd);
}

void exe_cgi(int sock,const char* path,const char* method,const char* query_string)
{
	printf("exe_cgi\n");
	int content_length = -1;
	if(strcasecmp(method,"get") == 0)
	{
		clear_header(sock);
	}
	else
	{
		char buf[SIZE];
		int ret = 0;
		do{
			ret = get_line(sock,buf,sizeof(buf));
			if(strncasecmp(buf,"Content-Length: ",16) == 0)
			{
				content_length = atoi(buf+16);
			}
		}while(ret >0 && strcmp(buf,"\n") != 0);

		if(content_length == -1)
			return;
	}

	const char* status_line="http/1.0 200 OK\r\n";
	//const char* type_line="Content-Type:text/html;charset=UTF-8\r\n";
	const char* type_line="Content-Type:text/html;charset=ISO-8859-1\r\n";
	const char* blank_line="\r\n";
	send(sock,status_line,strlen(status_line),0);
	send(sock,type_line,strlen(type_line),0);
	send(sock,blank_line,strlen(blank_line),0);
	
	int input[2];
	int output[2];
	if(pipe(input) < 0)
		return;
	if(pipe(output) < 0)
		return;
	char method_env[SIZE];
	char query_string_env[SIZE];
	char content_length_env[SIZE];

	pid_t id = fork();

	if(id < 0)
	{
		return;
	}
	else if(id == 0)
	{
		close(input[1]);
		close(output[0]);
		
		dup2(input[0],0);
		dup2(output[1],1);

		sprintf(method_env,"METHOD=%s",method);
		putenv(method_env);
		if(strcasecmp(method,"GET") == 0)
		{
			sprintf(query_string_env,"QUERY_STRING=%s",query_string);
			putenv(query_string_env);
		}
		if(strcasecmp(method,"POST") == 0)
		{
			sprintf(content_length_env,"CONTENT_LENGTH=%d",content_length);
			putenv(content_length_env);
		}
		execl(path,path,NULL);
		exit(1);
	}
	else
	{
		close(input[0]);
		close(output[1]);
		int i=0;
		char c='\0';
		if(strcasecmp(method,"POST") == 0)  
		{
			for(i=0; i<content_length; ++i)
			{
																						
				recv(sock,&c,1,0);    
				write(input[1],&c,1);   
			}
		}

		while(read(output[0],&c,1) > 0)   
		{
			send(sock,&c,1,0);    
		}
		waitpid(id,NULL,0);
		close(input[1]);
		close(output[0]);
	}
}



int  handle_request(int sock)
{
	printf("handle_request!\n");
	char buf[SIZE];
	char method[SIZE/10];
	char url[SIZE];
	char path[SIZE];
	int ret = 0;
	int nums = 0;
	int cgi = 0;
	char* query_string = NULL;
	//int fd = (int)arg;
#ifdef _DEBUG_
#endif
	if(get_line(sock,buf,sizeof(buf)) < 0)
	{
		printf("get_line is failed!\n");
		ret = -1;
		nums = 404;
		goto end;
	}
	printf("throw get_line\n");
	//GET /a/b/index.html http/1.0
	//先获取method   后获取url  再对method方法进行判断是否是get或者post方法  还有cgi方法   最后还要将参数进行分离
	//还要判断路径是否存在  不存在就404报错   路径存在还要看要获取的资源的method和size  
	//如果资源的路径是一个文件夹  那么就要在后面给上添加主页信息   资源是一个二进制文件就执行cgi
	//clear_header   echo_client   echo_www
	int i = 0;
	int j = 0;
	while(i < sizeof(method) - 1 && j < sizeof(buf) && !isspace(buf[j]))//获取method
	{
		method[i] = buf[j];
		i++,j++;

	}
	method[i] = 0;
	//获取url
	while(j < sizeof(buf) && isspace(buf[j]))
	{
		j++;
	}
	i = 0;
	while(i < sizeof(url) && j < sizeof(buf) && !isspace(buf[j]))
	{
		url[i] = buf[j];
		i++,j++;
	}
	url[i] = 0;
printf("method %s\n",method);
	//由于get和post有大小写之分  过滤非get和post的方法
	if(strcasecmp(method,"get") && strcasecmp(method,"post"))
	{
		ret = -2;
		nums = 404;
		goto end;
	}

	//get方法会将参数放在后面   要将参数进行分离
	if(strcasecmp(method,"post") == 0)
	{
		cgi = 1;
	}
	else
	{
		query_string = url;
		while(*query_string != '\0')
		{
			if(*query_string == '?')
			{
			*query_string = '\0';
			query_string++;
			cgi = 1;
			break;
		}
		query_string++;
		}
	}


	//把wwwroot追加在path的前面
	sprintf(path,"wwwroot%s",url);
	if(path[strlen(path)-1] == '/')
	{   
		strcat(path,"index.html");
	}

printf("path  %s\n",path);
	//然后再判断是否是一个文件夹   是否是二进制文件 
	struct stat st;
	if(stat(path,&st) < 0)
	{
		ret = -3;
		nums = 404;
		goto end;
	}
	else
	{
		if(S_ISDIR(st.st_mode))
		{
			strcat(path,"index.html");
		}
		else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
		{
			cgi = 1;
		}
		else
		{}
	}
	if(cgi == 1)
	{
		exe_cgi(sock,path,method,query_string);
	}
	else
	{
		clear_header(sock);
		echo_www(sock,path,st.st_size);
	}
	//最后再看是否是cgi处理还是普通的 
end:
	echo_client(nums);
	close(sock);
	return ret;
}

int get_line(int sock,char buf[],int len)
{
	printf("get_line\n");
	char c = '\0';
	int i = 0;
	while(c != '\n' && i < len-1)
	{
		ssize_t s = recv(sock,&c,1,0);
		if(s > 0)
		{
			if(c == '\r')
			{
				ssize_t m  = recv(sock,&c,1,MSG_PEEK);
				if((m > 0) && (c == '\n'))
				{
					recv(sock,&c,1,0);
				}
				else
					c = '\n';
			}
			buf[i++] = c;
		
		}
		else
		{
			printf("get_line is faild\n");
			c = '\n';
		}
	}
	buf[i] ='\0';
	return i;
}












