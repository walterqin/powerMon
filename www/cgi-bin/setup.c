#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cgic.h"
#include "sqlite3.h"

static char *keyname[] = {"Submit1", "Submit2", "Submit3", "Submit4", "Submit5", "Submit6", "Submit7", "Submit8", 
						  "Submit9", "Submit10", "Submit11", "Submit12", "Submit13", "Submit14", "Submit15", "Submit16", 
						  "Submit17"};
static char *txtname[] = {"cur1",     "cur2",    "vol1",       "vol2",       "ip", 
						 "gateway",  "email",   "dntime",     "uptime",     "dstatus", 
						 "bstatus",  "maxways", "serialattr", "serialport", "serialtype", 
						 "interval", "ontime",  "version"};
const char *dbkey[] = {"iAlarmLow",      "iAlarmUp",   "vAlarmLow",     "vAlarmUp",     "ip", 
					   "gateway",        "email",      "closeDateTime", "openDateTime", "keyDefault",
					   "keyStatus",      "maxWay",     "serialAttr",    "serialPort",   "serialType",
					   "switchInterval", "timeEnable", "version"};

void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>result</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1><p align=\"center\" >INFO: %s </p>\n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}
					   
int cgiMain()
{
	int i;
	char *dbname = "/root/db/profile.db";
	sqlite3 *db;
	int rc = SQLITE_OK;
	char *errmsg = 0;
	char attr[256] = {0};
	char sql[256] = {0};
	char strtmp[256] = {0};
	
//	CookieSet();
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "\n\n");
	
	/* Initialize database */
	memset(strtmp, 0, 256);
	rc = sqlite3_open(dbname, &db);  
	if (rc)  
	{  
	    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));  
	    sqlite3_close(db); 
		sprintf(strtmp, "Open database failure!");
		printErrorInfo(strtmp);
	    return -1;  
	}
	
//	memset((char *)attr, 0, 256);
//	memset((char *)sql, 0, 256);
	for (i = 0; i < 17; i++)
	{
		if (cgiFormSubmitClicked(keyname[i]) == cgiFormSuccess)
		{
			cgiFormStringNoNewlines(txtname[i], attr, 256);
			sprintf(sql, "update profile set value='%s' where name='%s'", attr, dbkey[i]);
			rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
			if (rc != SQLITE_OK)
			{
				fprintf(stderr, "sql error:%s\n", errmsg);
				sqlite3_free(errmsg);
				sqlite3_close(db);
				sprintf(strtmp, "Update parameter error!");
				printErrorInfo(strtmp);
				return -1;
			}
			break;
		}
	}
	
	sqlite3_free(errmsg);
	sqlite3_close(db);
	
	sprintf(strtmp, "Operation success!");
	printErrorInfo(strtmp);
	
	return 0;
}
