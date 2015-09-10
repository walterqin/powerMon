#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>  
#include <unistd.h>
#include <limits.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <error.h>
#include "cgic.h"

#define MAXWAY 8
#define PROLEN 8
#define RELAYADDR 2
#define FIFO_NAME "/tmp/my_fifo"
#define FIFO_SIZE 8
#define FLAG_DIR "/tmp/"

char flag_dir[] = "/tmp/";

enum cmdid
{
	CmdId_Open1 = 0,
	CmdId_Open2,
	CmdId_Open3,
	CmdId_Open4,
	CmdId_Open5,
	CmdId_Open6,
	CmdId_Open7,
	CmdId_Open8,
	CmdId_Close1,
	CmdId_Close2,
	CmdId_Close3,
	CmdId_Close4,
	CmdId_Close5,
	CmdId_Close6,
	CmdId_Close7,
	CmdId_Close8,
};

static char *openCmd[] = {"open1", "open2", "open3", "open4", "open5", "open6", "open7", "open8", NULL};
static char *closeCmd[] = {"close1", "close2", "close3", "close4", "close5", "close6", "close7", "close8", NULL};

void printErrorInfo(char *info)
{
	fprintf(cgiOut, "<HTML><HEAD>\n"); 
	fprintf(cgiOut, "<TITLE>result</TITLE></HEAD>\n"); 
	fprintf(cgiOut, "<hr /><BODY><H1><p align=\"center\" >INFO: %s </p>\n", info); 
	fprintf(cgiOut, "</BODY></HTML>\n"); 
}

int cgiMain()
{
	char openflag[128] = {0};
	char closeflag[128] = {0};
	char info[256] = {0};
	int i = 0;
	int id = 0;
	
//	CookieSet();
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "\n");
	
	//按键处理
	for ( i = 0; i < MAXWAY; i++)
	{
		id = i + 1;
		if (cgiFormSubmitClicked(openCmd[i]) == cgiFormSuccess)
		{
			strcpy(openflag, flag_dir);
			strcat(openflag, openCmd[i]);
			if (open(openflag, O_CREAT | O_RDWR) < 0)
			{
				sprintf(info, "Open switch #%d failure!\n", id);
				printErrorInfo(info);
			}
			else
			{
				sprintf(info, "Open switch #%d success!\n", id);
				printErrorInfo(info);
			}
			break;
		}
		
		if (cgiFormSubmitClicked(closeCmd[i]) == cgiFormSuccess)
		{
			strcpy(closeflag, flag_dir);
			strcat(closeflag, closeCmd[i]);
			if (open(closeflag, O_CREAT | O_RDWR) < 0)
			{
				sprintf(info, "Close switch #%d failure!\n", id);
				printErrorInfo(info);
			}
			else
			{
				sprintf(info, "Close switch #%d success!\n", id);
				printErrorInfo(info);
			}
			break;
		}
	}

	return 0;
}
