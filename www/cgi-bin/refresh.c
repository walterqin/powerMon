#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cgic.h"
#include "sqlite3.h"

#define MAXWAY 8

typedef struct
{
	char volt[32];
	char curr[32];
	char stat[4];
} PowerPara;

void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>result</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1><p align=\"center\" >INFO: %s </p>\n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}
int queryinfo(void *para, int col, char **colvalue, char **colname)
{
	PowerPara *pp = (PowerPara *)para;
	if (col > 2)
	{
		strcpy(pp->volt, colvalue[0]);
		strcpy(pp->curr, colvalue[1]);
		strcpy(pp->stat, colvalue[2]);	
	}
	return 0;
}

int cgiMain()
{
	char *dbname = "/root/db/data.db";
	char sql[256] = {0};
	sqlite3 *db;
	int rc = SQLITE_OK;
	char *errmsg;
	PowerPara pp[8];
	char info[256] = {0};
	char result[1024] = {0};
	char tmp[256] = {0};
	int i = 0;

	cgiHeaderContentType("text/html");

	rc = sqlite3_open(dbname, &db); 
	if (rc != SQLITE_OK)  
	{  
		memset(info, 0, 256);
		sprintf(info, "Can't open database: %s\n", dbname); 
		printErrorInfo(info);
		return -1;  
	}

	for (i = 0; i < MAXWAY; i++)
	{
		memset(sql, 0, 256);
		sprintf(sql, "select volt, curr, switch from power where waynum = '%d'", i + 1);
		rc = sqlite3_exec(db, sql, queryinfo, &pp[i], &errmsg);	
		if (rc != SQLITE_OK)
		{
			memset(info, 0, 256);
			sprintf(info, "Get data failure!%s,%d\n", errmsg, rc);
			printErrorInfo(info);
			sqlite3_close(db);
			return -1;
		}
	}
	
	sqlite3_close(db);

	for (i = 0; i < MAXWAY; i++)
	{
		sprintf(tmp,  "volt%d=%s&curr%d=%s&stat%d=%s&", i + 1, pp[i].volt, i + 1, pp[i].curr,  i + 1, pp[i].stat);
		strcat(result, tmp);
	}
	fprintf(cgiOut, result);
	
	return 0;
}

