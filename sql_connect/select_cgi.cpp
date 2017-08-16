#include"sql_api.h"

int main()
{
	sql_api sql;
	sql.connect();
	sql.select();
	return 0;
}
