#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cgic.h"
#include "sqlite3.h"

const char *dbname = "/root/db/profile.db";

void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>result</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1><p align=\"center\" >INFO: %s </p>\n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}

int queryinfo(void *para, int col, char **colvalue, char **colname)
{
	*(int *)para = 1;
	return 0;
}

int cgiMain()
{
	sqlite3 *db;
	int rc = SQLITE_OK;
	char *szErrMsg = 0;
	char sql[256] = {0};
	char name[128] = {0};
	char passwd[128] = {0};
	char info[128] = {0};
	int empty = 0;

	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "\n\n");

	rc = sqlite3_open(dbname, &db);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "Cannot open database: %s", dbname);
		sqlite3_close(db);
		sprintf(info, "Open database failure!");
		printErrorInfo(info);
		return -1;
	}

	if (cgiFormSubmitClicked("adduser") == cgiFormSuccess)
	{
		cgiFormStringNoNewlines("username", name, 128);
		cgiFormStringNoNewlines("passwd", passwd, 128);

		sprintf(sql, "select * from user where name = '%s'", name);
		rc = sqlite3_exec(db, sql, queryinfo, &empty, &szErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Execure '%s' false!", sql);
			sqlite3_close(db);
			sprintf(info, "Execure '%s' failure!", sql);
			printErrorInfo(info);
			return -1;
		}
		
		memset(sql, 0, 128);
		if (empty == 0)
			sprintf(sql, "insert into user (name, passwd) values ('%s', '%s')", name, passwd);
		else
			sprintf(sql, "update user set passwd = '%s' where name = '%s'", passwd, name);
		
		rc = sqlite3_exec(db, sql, NULL, NULL, &szErrMsg);
		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "Execure '%s' false!, %s", sql, szErrMsg);
			sqlite3_close(db);
			sprintf(info, "Execure '%s' failure!, %s", sql, szErrMsg);
			printErrorInfo(info);
			return -1;
		}
	}
	return 0;
}
