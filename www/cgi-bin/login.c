#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgic.h"
#include "sqlite3.h"

const char *dbname = "/root/db/profile.db";

struct result
{
	char name[128];
	char passwd[128];
};


void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>result</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1><p align=\"center\" >INFO: %s </p>\n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}

int queryinfo(void *para, int col, char **colvalue, char **colname)
{
	struct result *res = (struct result *)para;
	sprintf(res->name, "%s", colvalue[0]);
	sprintf(res->passwd, "%s", colvalue[1]);
	return 0;
}
int cgiMain() 
{
	char name[241] = {0};
	char password[241] = {0};
	char myname[128] = {0};
	char mypswd[128] = {0};
	char sql[256] = {0};
	int rc = SQLITE_OK;
	char *szerrmsg = NULL;
	sqlite3 *db;
	struct result res;

	cgiHeaderContentType("text/html");

	cgiFormString("password", password,  241);
	cgiFormString("name", name, 241);

	memset(res.name, 0, 128);
	memset(res.passwd, 0, 128);

	rc = sqlite3_open(dbname, &db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s", dbname);
		sqlite3_close(db);
	}

	sprintf(sql, "select name, passwd from user");
	rc = sqlite3_exec(db, sql, queryinfo, &res, &szerrmsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Execure '%s' failure!", sql);
		sqlite3_close(db);
	}
	else
	{
		strncpy(myname, res.name, 128);
		strncpy(mypswd, res.passwd, 128);
	}

	if (!strcmp(res.name, "") && !strcmp(res.passwd, ""))
	{
		strncpy(myname, "admin",  6);
		strncpy(mypswd, "63324455", 9);
	}

	if (!strcmp(name, myname) && !strcmp(password, mypswd))
	{
		fprintf(cgiOut, "<html>\n");
		fprintf(cgiOut, "<head><title>login success</title></head>\n");
		fprintf(cgiOut, "<meta http-equiv=\"Refresh\" content=\"0;URL=http://192.168.1.253/main.htm\">\n");
	}
	else
	{
		memset(sql, 0, 256);
		sprintf(sql, "User Name or Password Error!\n");
		printErrorInfo(sql);
		sqlite3_close(db);
		return -1;
	}	

	return 0;
}
