#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cgic.h"
#include "sqlite3.h"

typedef int bool;
#define FALSE 0
#define TRUE 1

#define BUFSIZE 4096

static char bufferFile[BUFSIZE];
static int bufferActualSize = BUFSIZE; 
// static int nEOB = FALSE;
static int strHead = 0;
// static int strTail = -1;
static int num = 1;

const char *templateDir="/www/cgi-bin/template/";
const char *dbname = "/root/db/data.db";

sqlite3 *db;

static int fillBufferFile(FILE *fp);
static bool initReadString(FILE *fp);
static int readString(char *buffile, char *buf, unsigned int strStart, unsigned int max);
static int queryDatabase(char *waynum, char *startdata, char *stopdata);
static void printWebPageTableTitle();
static void printWebPagePageTableEndline();
static int loadInfo(void *para, int column, char **column_value, char **column_name);
static void printErrorInfo(char *info);

int cgiMain()
{
//	int i;
	char templateFileName[128] = {0};
//	char attr[256] = {0};
	char buf[BUFSIZE] = {0};
	char waynum[32] = {0};
	char startdate[32] = {0};
	char stopdate[32] = {0};
	char errmsg[256] = {0};
	int count = 0;
	FILE *fp;
	
	//CookieSet();
	cgiHeaderContentType("text/html");
	
	if (cgiFormSubmitClicked("submit") == cgiFormSuccess)
	{
		cgiFormStringNoNewlines("waynum", waynum, 32);
		cgiFormStringNoNewlines("startdate", startdate, 32);
		cgiFormStringNoNewlines("stopdate", stopdate, 32);
		
		strHead = 0;
		num  = 1;
	}
	else
		return 0;
	
	strcpy(templateFileName, templateDir);
	strcat(templateFileName, "query.htm");
	if ((fp = fopen(templateFileName, "rw+")) < 0 )
	{
		memset(errmsg, 0, 256);
		sprintf(errmsg, "Cannot open template file %s\n", templateFileName);
		printErrorInfo(errmsg);
		return -1;
	}
	
	/* print web page , fill "query data" by the flag */
	if ((bufferActualSize = initReadString(fp)) >= 0)
	{
		while (1)
		{
			memset(buf, 0, BUFSIZE);
			count = readString(bufferFile, buf, strHead, BUFSIZE);
			if (count < 0)
			{
				fclose(fp); 
				printErrorInfo("Read file error!");
				return -1;
			}
			else if (count > 0)
			{
				strHead += count;

				/* find flag <!-- query data --> */
				if (strstr(buf, "<!-- query data -->") != NULL)
				{
					printWebPageTableTitle();
					if (queryDatabase(waynum, startdate, stopdate) < 0)
					{
						fprintf(cgiOut, "query database error");
						printWebPagePageTableEndline();
						continue;
					}
					else
						printWebPagePageTableEndline();

				}
				else
				{
					fprintf(cgiOut, buf, count);
					fprintf(cgiOut, "\n");
				}
			}
			else
				break;
		}
	}
	else
	{
		fclose(fp); 
		printErrorInfo("Read file error!");
		return -1;
	}
	return 0;	
}

void printWebPageTableTitle()
{
	fprintf(cgiOut, "<div class=\"scroll_layer\">\n");
	fprintf(cgiOut, "<table border=\"1\" align=\"center\" bordercolor=\"#0066ff\" cellspacing=\"0\" style=\"width:680px; border-collapse:collapse\" class=\"style4\">\n");
	fprintf(cgiOut, "<tr>\n");
	fprintf(cgiOut, "<td align=\"center\">序号</td>\n");
	fprintf(cgiOut, "<td align=\"center\">路号</td>\n");
	fprintf(cgiOut, "<td align=\"center\">电压值</td>\n");
	fprintf(cgiOut, "<td align=\"center\">电流值</td>\n");
	fprintf(cgiOut, "<td align=\"center\">日期</td>\n");
	fprintf(cgiOut, "<td align=\"center\">时间</td>\n");
	fprintf(cgiOut, "</tr>\n");
}

void printWebPagePageTableEndline()
{
	fprintf(cgiOut, "</table>\n");
	fprintf(cgiOut, "</div>\n");
}

/**
* The callback function by sqlite query  
*/
int loadInfo(void *para, int column, char **column_value, char **column_name)
{
	int i;
	
	fprintf(cgiOut, "<tr>\n");
	fprintf(cgiOut, "<td class=\"fixed\" align=\"center\">%d</td>\n", num);
	for (i = 0; i < column; i++)
		fprintf(cgiOut, "<td class=\"fixed\" align=\"center\">%s</td>\n", column_value[i]);
	fprintf(cgiOut, "</tr>\n");

	num++;
	return 0;
}

int queryDatabase(char *waynum, char *startdate, char *stopdate)
{
	int rc = SQLITE_OK;
	char *szErrMsg = 0;
	char sql[256];
//	char errmsg[256];
	char strtmp[128];
	
	rc = sqlite3_open(dbname, &db); 
	if (rc != SQLITE_OK)  
	{  
		fprintf(cgiOut, "Can't open database: %s\n", sqlite3_errmsg(db)); 
		return -1;  
	}

	memset(sql, 0, 256);
	if (!strcmp(waynum, "") && !strcmp(startdate, "") && !strcmp(stopdate, ""))
	{
		strcpy(sql, "select * from result");
	}
	else
	{
		strcpy(sql, "select waynum, volt, curr, date, time from result where ");
	
		memset(strtmp, 0, 128);
		if (strcmp(waynum, "") != 0)
		{
			sprintf(strtmp, "waynum = '%s' ", waynum);
			strcat(sql, strtmp);
		}
		
		memset(strtmp, 0, 128);
		if (strcmp(startdate, "") != 0)
		{
			sprintf(strtmp, "and date > '%s' ", startdate);
			strcat(sql, strtmp);
		}
		
		memset(strtmp, 0, 128);
		if (strcmp(stopdate, "") != 0)
		{
			sprintf(strtmp, "and date <= '%s'", stopdate);
			strcat(sql, strtmp);
		}
	}
	rc = sqlite3_exec(db, sql, loadInfo, NULL, &szErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(cgiOut, "Query database error: %s\n", szErrMsg); 
		return -1;
	}
	return 0;
}

void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>ERROR</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1>ERROR: %s \n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}

int fillBufferFile(FILE *fp)
{
	int count = 0;
	int readlen = 0;
	while (readlen < BUFSIZE)
	{
		count = fread(bufferFile + readlen, sizeof(char), BUFSIZE - readlen, fp);
		if (count < 0)
		{
			fprintf(stderr, "fillBufferFile: read file failed!\n");
			return -1;
		}
		else
		{
			readlen += count;
			if (feof(fp))
				break;
		}
	}
	return readlen;
}

int initReadString(FILE *fp)
{
	int n = 0;
	if (fp == NULL)
		return -1;
		
	//nEOB = FALSE;
	n = fillBufferFile(fp);
	if (n < 0)
		return -1;
	else
		return n;
}

int readString(char *buffile, char *buf, unsigned int strStart, unsigned int max)
{
	unsigned int count = 0;
//	unsigned int n = 0;
	unsigned int strHead = strStart;
	unsigned int strTail = strStart - 1;
	
	if (bufferFile == NULL || buf == NULL || max == 0)
		return -1;
	if (strStart >= bufferActualSize)
		return 0;
	
	strTail = strHead + 1;
	if (buffile[strHead] == 0x0a)
		strHead++;
	while (++strTail < bufferActualSize) 
	{
		if (buffile[strTail]== 0x0d)
		{
			count = strTail - strHead;
			if (count > max)
				return -1;
			memcpy(buf, buffile + strHead,  count);
			return count;
		}
	}
	
	count  = bufferActualSize - strHead;
	if (count > max)
		return -1;
	memcpy(buf, buffile + strHead, count);
	return count;
}
