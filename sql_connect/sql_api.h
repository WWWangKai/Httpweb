#ifndef _SQL_API_H_
#define _SQL_API_H_

#include<iostream>
using namespace std;
#include<string>
#include<mysql.h>
#include<stdlib.h>
#include<string>
#include<unistd.h>
#include<stdio.h>
#include<cstring>

class sql_api
{
public:
	sql_api();
	int connect();
	int insert(const string& name,const string& sex,const string& school,const string& hobby);
	void select();
	~sql_api();
private:
	MYSQL* conn;
};


#endif

