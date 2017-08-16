// wxsqlcode.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "sqlite3.h"
#include <string.h>
#include <string>

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("USAGE: wxsqlcode [data path] [password]\n");
		return 0;
	}

	std::string name = (std::string)argv[1];

	char* dbname = argv[1];
	char* passwd = argv[2];
	sqlite3* pdb = NULL;

	int err = sqlite3_open(dbname, &pdb);
	if(err != SQLITE_OK)
	{
		printf("can not open %s \n",dbname);
		printf("sqlite3_open error code : %d\n",err);
		return false;
	}

	err = sqlite3_rekey(pdb,passwd,strlen(passwd));
	if(err != SQLITE_OK)
	{
		printf("sqlite3_rekey error code : %d\n",err);
		return 0;
	}

	sqlite3_close(pdb);
	return 0;
}

