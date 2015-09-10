#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sqlite3.h>

//#include "errlog.h"
#include "rs485.h"
//#include "daemon.h"
//
#define BAUD_RATE B9600
#define BUF_SIZE  128 
#define END_FLAG  0x23
#define FIFO_NAME "/tmp/my_fifo"

char *relayup[MAXWAY] = {"/tmp/open1", "/tmp/open2", "/tmp/open3", "/tmp/open4", 
						 "/tmp/open5", "/tmp/open6", "/tmp/open7","/tmp/open8"};
char *relaydn[MAXWAY] = {"/tmp/close1", "/tmp/close2", "/tmp/close3", "/tmp/close4", 
						 "/tmp/close5", "/tmp/close6", "/tmp/close7", "/tmp/close8"};

static int  querypower = 1;

/** used by 'powerMon' program communication, because qt don't support 'FIFO' ipc */
unsigned char relayopt(char *flag[])
{
	int i;
	unsigned char mask = 1;
	unsigned char ret = 0;
	for (i = 0; i < MAXWAY; i++)
	{
		if (access(flag[i], F_OK) == 0)
		{
			ret |= (mask << i);
		}
	}
		
	return ret;
}

/** @brief query power parame function*/
void query_info()
{
	struct command cmd;
	// pthread_t tid;
	
	if (querypower == 1)
	{
		rs485_getcommand(CmdId_ReadAllPP, &cmd);
		rs485_run(&cmd);
		// pthread_create(&tid, NULL, (void*)rs485_run, &cmd);
		// rs485_sendcommand(CmdId_ReadAllPP);
		querypower = 0;
	}
	else
	{
		rs485_getcommand(CmdId_RelayQueryStatus, &cmd);
		rs485_run(&cmd);
		// pthread_create(&tid, NULL, (void *)rs485_run, &cmd);
		// rs485_sendcommand(CmdId_RelayQueryStatus);
		querypower = 1;
	}
}

/** @brief initial singal handler */
void init_sigaction(void)
{
    struct sigaction tact;
    tact.sa_handler = query_info;
    tact.sa_flags = 0;
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
}

/** @brief initial timer, interval: 20s */
void init_time()
{
    struct itimerval value;
    value.it_value.tv_sec = 20;
    value.it_value.tv_usec = 0;
    value.it_interval = value.it_value;
    setitimer(ITIMER_REAL, &value, NULL);
}

sqlite3 *db;

int main(int argc, char *argv[])
{
	char *tty_dev = "/dev/ttySAC3"; // Default rs485 use ttySAC3
	char *dbname = "/root/db/data.db";  // Default database path
	unsigned char msg[2];
	int i = 0;
	int ffd;
	int rc;
	pthread_t id;
	struct command cmd;
	int cmdid;
	unsigned char ret = 0;
	unsigned char mask = 1;

	printf("Initialize start.....\n");

	/* Get device node from input parameter */
	if (argc > 1) {
		tty_dev = argv[1];
	}
	
	/* Initialize database */
	rc = sqlite3_open(dbname, &db);  
	if (rc != SQLITE_OK)  
	{  
	    fprintf(stderr, "Can't open database\n");  
	    sqlite3_close(db); 
		rs485_exit();
	    return -1;  
	}
	fprintf(stderr, "Initial database success!\n");

	/* Initialize device */
	if (rs485_init(tty_dev, BAUD_RATE) < 0) {
		fprintf(stderr, "Exit now\n");
		return (-1);
	}
	fprintf(stderr, "Initialize serial %s success!\n", tty_dev);
	
	/* Initialize timer, query power parameter per 5s */
	init_sigaction();
    init_time();
	fprintf(stderr, "Initial timer success!\n");
	
	/* Initialize process communication channel FIFO */  
    if((mkfifo(FIFO_NAME, O_CREAT|O_EXCL) < 0) && (errno != EEXIST))  
        fprintf(stderr, "cannot create fifoserver\n");

    ffd = open(FIFO_NAME, O_RDONLY|O_NONBLOCK, 0);  
    if(ffd == -1)  
        fprintf(stderr, "open fifo false!");  
	fprintf(stderr, "Initial fifo success!\n"); 

	/* Deal message by other process send */
	fprintf(stderr, "Start message process......\n");
	while (1)
	{
		memset(msg, 0, sizeof(msg));
		if (read(ffd, msg, 2) > 0)
		{
			rs485_getcommand((int)(msg[0]), &cmd);
			rs485_run(&cmd);
			// /* Create a thread for receive message */
			// pthread_create(&id, NULL, (void*)rs485_run, &cmd);
			// rs485_sendcommand((int)(msg[0]));
		}
		
		if ((ret = relayopt(relayup)) > 0)
		{
			for (i = 0; i < 8; i++)
			{
				cmdid = ret & (mask << i);
				if (cmdid > 0)
				{
					rs485_getcommand(CmdId_Open1 + i, &cmd);
					rs485_run(&cmd);
					remove(relayup[i]);
				}
			//	fprintf(stdout, "open: cmdid=%d, ret=%d\n", cmdid, ret);
			}
		}	

		if ((ret = relayopt(relaydn)) > 0)
		{
			for (i = 0; i < 8; i++)
			{
				cmdid = ret & (mask << i);
				if (cmdid > 0)
				{
					rs485_getcommand(CmdId_Close1 + i, &cmd);
					rs485_run(&cmd);
					remove(relaydn[i]);
				}
			//	fprintf(stdout, "close: cmdid=%d, ret=%d", cmdid, ret);
			}
		}
	}

	/* close fifo */
	close(ffd);
	fprintf(stderr, "Close fifo ok!\n");
	
	/* close database */
	sqlite3_close(db);
	fprintf(stderr, "Close database ok!\n");
	
	/* Exit rs485 */
	rs485_exit();
	fprintf(stderr, "Close serial ok!\n");
	
	return 0 ;
}
