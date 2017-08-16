#ifndef _HTTPD_H_
#define _HTTPD_H_
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<unistd.h>
#include <sys/sendfile.h>
#include<unistd.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
#include<string.h>


#define SIZE 1024
int startup(char* ip,int port);
int handle_request(int sock);
int get_line(int sock,char buf[],int len);
void clear_header(int sock);
void echo_client(int nums);
void echo_www(int sock,char *path,int size);
void exe_cgi(int sock,const char* path,const char* method,const char* query_string);

#endif
