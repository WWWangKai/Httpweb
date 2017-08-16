#include"sql_api.h"
using namespace std;

sql_api::sql_api()
{
	conn = mysql_init(NULL);
}

int sql_api::connect()
{
	if(NULL == mysql_real_connect(conn,"127.0.0.1","root",NULL,"class",3306,"NULL",0))
	{
		cerr<<"<h3>connect error!</h3><br/>"<<endl;
	}
	else
	{
		cout<<"<h3>Connect Success!</h3><br/>"<<endl;
	}
}

int sql_api::insert(const string &name,const string &sex,const string &school,const string &hobby)
{
	string sql = "INSERT INTO sql_table (name,sex,school,hobby) VALUES('";
	sql += name;
	sql += "','";
	sql += sex;
	sql += "','";
	sql += school;
	sql += "','";
	sql += hobby;
	sql += "')";
	int ret = mysql_query(conn,sql.c_str());
	if(ret == 0 )
	{
		cout<<"<h3>Insert success!</h3><br/>"<<endl;
	}
	else
	{
		cout<<"<h3>insert faild!</h3><br/>"<<endl;
	}
		
}

void sql_api::select()
{
	string sql = "SELECT * FROM sql_table"; /*数据库要执行的命令*/
	int ret = mysql_query(conn,sql.c_str());//这个函数用来执行mysql指令
	if(ret == 0)
	{
		//mysql的数据控制
		MYSQL_RES *res = mysql_store_result(conn);//成功返回一个指向返回集的指针
		if(res)
		{
			int lines = mysql_num_rows(res);//获取返回集中的行数
			int rows = mysql_num_fields(res);
			cout<<"<h3>lines=: "<<lines<<"</h3>"<<"<h3>rows=: "<<rows<<"</h3>"<<endl;
			cout<<endl<<"<br/>";
			MYSQL_FIELD *field = NULL;
			
			cout<<"<h2>";
			for(;field = mysql_fetch_field(res);)//获得与列字段相关信息的结构指针
			{
				cout<<field->name<<"	";
			}
			cout<<endl<<"</h2><br/>";

			int i , j;
			for(i = 0 ;i < lines; i++)
			{
				cout<<"<h3>";
				MYSQL_ROW row = mysql_fetch_row(res);//从mysql_store_resul返回集中取一行  没有数据null
				//会返回一个MYSQL_ROW变量 其实就是一个char** 把其当成一个二维数组
				for(j = 0;j < rows;j++)
				{
					cout<<row[j]<<endl;
				}
				cout<<"</h3>"<<endl;
			}
			cout<<"<br/>";
		}
		cout<<endl;
		free(res);
	}
}

sql_api::~sql_api()
{
	mysql_close(conn);
}







