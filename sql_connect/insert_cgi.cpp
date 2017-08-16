#include"sql_api.h"
#include<stdlib.h>
#include<strings.h>

void insert_data(char* data_string)
{
	//cout<<"data_string"<<data_string<<endl;
	char* myargv[5];
	char* start = data_string;
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
	sql_api sql;
	sql.connect();
	cout<<"<h3>Check your information!</h3><br/>"<<endl;
	cout<<"<h4>"<<"name: "<<myargv[0]<<"<br/>"<<endl;
	cout<<"sex: "<<myargv[1]<<"<br/>"<<endl;
	cout<<"school: "<<myargv[2]<<"<br/>"<<endl;
	cout<<"hobby: "<<myargv[3]<<"</h4><br/>"<<endl;
	//cout<<"connect success!"<<endl;
	sql.insert(myargv[0],myargv[1],myargv[2],myargv[3]);
}
int main()
{
	
	char* method = getenv("METHOD");
	char* data_string = NULL;
	char buf[1024];
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
		insert_data(data_string);
	}
	return 0;
}

