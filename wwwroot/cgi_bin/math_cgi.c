#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define SIZE 1024

void mymath(char* data_string)
{
	printf("data_string %s\n",data_string);
	char *myargv[3];
	char *start = data_string;
	int i = 0;
	while(*start)
	{
		if(*start == '=')
		{
			myargv[i++] = start + 1;
		}
		if(*start == '&')
		{
			*start = '\0';
		}
		start++;
	}
	int data1 = atoi(myargv[0]);
	int data2 = atoi(myargv[1]);
	printf("\n");
	printf("data1:%d  data:%d\n",data1,data2);


	printf("<html>");
	printf("<h1>%d + %d = %d</h1><br/>",data1,data2,data1+data2);
	printf("<h1>%d - %d = %d</h1><br/>",data1,data2,data1-data2);
	printf("<h1>%d * %d = %d</h1><br/>",data1,data2,data1*data2);
	printf("<h1>%d / %d = %d</h1><br/>",data1,data2,data1/data2);
	printf("<h1>%d %% %d = %d</h1><br/>",data1,data2,data1%data2);
	printf("</html>");

}


int main()
{
	char* method = getenv("METHOD");
	char* data_string = NULL;
	char buf[SIZE];
	if(strcasecmp(method,"get") == 0)
	{
		data_string = getenv("QUERY_STRING");
	}
	else
	{
		char* content_len = getenv("CONTENT_LENGTH");
		int len = atoi(content_len);
		char c;
		int i = 0;
		for(; i < len ;i++)
		{
			read(0,&c,1);
			buf[i] = c;
		}
		buf[i] = 0;
		data_string = buf;
	}
	if(data_string)
	{
		mymath(data_string);
	}
	return 0;
}







