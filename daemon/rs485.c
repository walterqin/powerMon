#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>
#include <sqlite3.h>

#include "rs485.h"

unsigned char relayopen[MAXWAY][PROLEN] = 
{
	{0x02, 0x05, 0x0, 0x0, 0xff, 0x0, 0x8c, 0x09}, //打开1路继电器
	{0x02, 0x05, 0x0, 0x1, 0xff, 0x0, 0xdd, 0xc9}, //打开2路继电器
	{0x02, 0x05, 0x0, 0x2, 0xff, 0x0, 0x2d, 0xc9}, //打开3路继电器
	{0x02, 0x05, 0x0, 0x3, 0xff, 0x0, 0x7c, 0x09}, //打开4路继电器
	{0x02, 0x05, 0x0, 0x4, 0xff, 0x0, 0xcd, 0xc8}, //打开5路继电器
	{0x02, 0x05, 0x0, 0x5, 0xff, 0x0, 0x9c, 0x08}, //打开6路继电器
	{0x00, 0x00, 0x0, 0x0, 0x00, 0x0, 0x00, 0x1b}, //打开7路继电器
	{0x00, 0x00, 0x0, 0x0, 0x00, 0x0, 0x00, 0x1b}  //打开8路继电器
};

unsigned char relayclose[MAXWAY][PROLEN] =
{
	{0x02, 0x05, 0x0, 0x0, 0x0, 0x0, 0xcd, 0xf9}, //关闭1路继电器
	{0x02, 0x05, 0x0, 0x1, 0x0, 0x0, 0x9c, 0x39}, //关闭2路继电器
	{0x02, 0x05, 0x0, 0x2, 0x0, 0x0, 0x6c, 0x39}, //关闭3路继电器
	{0x02, 0x05, 0x0, 0x3, 0x0, 0x0, 0x3d, 0xf9}, //关闭4路继电器
	{0x02, 0x05, 0x0, 0x4, 0x0, 0x0, 0x8c, 0x38}, //关闭5路继电器
	{0x02, 0x05, 0x0, 0x5, 0x0, 0x0, 0xdd, 0xf8}, //关闭6路继电器
	{0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x00, 0x1b}, //关闭7路继电器
	{0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x00, 0x1b}  //关闭8路继电器
};

unsigned char readpp[MAXWAY][PROLEN] = 
{
	{0x01, 0x03, 0x0, 0x40, 0x0, 0x02, 0xc5, 0xdf}, //读取1路电压电流参数
	{0x01, 0x03, 0x0, 0x47, 0x0, 0x02, 0x74, 0x1e}, //读取2路电压电流参数
	{0x01, 0x03, 0x0, 0x4e, 0x0, 0x02, 0xa4, 0x1c}, //读取3路电压电流参数
	{0x01, 0x03, 0x0, 0x55, 0x0, 0x02, 0xd4, 0x1b}, //读取4路电压电流参数
	{0x01, 0x03, 0x0, 0x5c, 0x0, 0x02, 0x04, 0x19}, //读取5路电压电流参数
	{0x01, 0x03, 0x0, 0x63, 0x0, 0x02, 0x34, 0x15}, //读取6路电压电流参数
	{0x01, 0x03, 0x0, 0x02, 0x0, 0x01, 0x25, 0xca}, //读取电压量程
	{0x01, 0x03, 0x0, 0x03, 0x0, 0x01, 0x74, 0x0a}  //读取电流量程
};

unsigned char readallpp[] = {0x01, 0x03, 0x0, 0x40, 0x0, 0x2a, 0xc5, 0xc1};                	//读取所有六路电压电流参数
unsigned char readid1[] = {0x01, 0x03, 0x0, 0x0, 0x0, 0x01, 0x84, 0x0a};                     	//读取设备id
unsigned char readbaud1[] = {0x01, 0x03, 0x0, 0x04, 0x0, 0x01, 0xc5, 0xcb};                  	//读取参数采集板的波特率
unsigned char setbaud1[] = {0x01, 0x10, 0x0, 0x04, 0x0, 0x01, 0x02, 0x01, 0x06, 0x26, 0x46}; 	//设置参数采集板波特率为9600
unsigned char querystatus[] = {0x02, 0x01, 0x0, 0x0, 0x0, 0x06, 0xbc, 0x3b};				  	//查询继电器状态
unsigned char querycoople[] = {0x02, 0x02, 0x0, 0x0, 0x0, 0x06, 0xf8, 0x3b};
unsigned char flashopen[] = {0x02, 0x10, 0x0, 0x03, 0x0, 0x02, 0x04, 0x0, 0x04, 0x0, 0x0a, 0x7d, 0x38};
unsigned char flashclose[] = {0x02, 0x10, 0x0, 0x03, 0x0, 0x02, 0x04, 0x0, 0x02, 0x0, 0x14, 0x1d, 0x31};
unsigned char allopen[] = {0x02, 0x0f, 0x0, 0x0, 0x0, 0x06, 0x01, 0xff, 0x9f, 0x03};
unsigned char allclose[] = {0x02, 0x0f, 0x0, 0x0, 0x0, 0x06, 0x01, 0x00, 0xdf, 0x43}; 

enum
{
	SendCycles = 5,		/* repeatedly send count */
	ReadCycles = 20,	/* repeatedly read count */
	BufferLen = 5,		/* command buffer length */
	RecBufLen = 128,    /* receive buffer length */
};

/* Device file description */
int fd;

static struct command cmdlist[] = {
	{CmdId_Open1, relayopen[0], 8},
	{CmdId_Open2, relayopen[1], 8},
	{CmdId_Open3, relayopen[2], 8},
	{CmdId_Open4, relayopen[3], 8},
	{CmdId_Open5, relayopen[4], 8},
	{CmdId_Open6, relayopen[5], 8},
	{CmdId_Open7, relayopen[6], 8},
	{CmdId_Open8, relayopen[7], 8},
	{CmdId_Close1, relayclose[0], 8},
	{CmdId_Close2, relayclose[1], 8},
	{CmdId_Close3, relayclose[2], 8},
	{CmdId_Close4, relayclose[3], 8},
	{CmdId_Close5, relayclose[4], 8},
	{CmdId_Close6, relayclose[5], 8},
	{CmdId_Close7, relayclose[6], 8},
	{CmdId_Close8, relayclose[7], 8},
	{CmdId_Read1Way, readpp[0], 8},
	{CmdId_Read2Way, readpp[1], 8},
	{CmdId_Read3Way, readpp[2], 8},
	{CmdId_Read4Way, readpp[3], 8},
	{CmdId_Read5Way, readpp[4], 8},
	{CmdId_Read6Way, readpp[5], 8},
	{CmdId_ReadVol, readpp[6], 8},
	{CmdId_ReadCur, readpp[7], 8},
	{CmdId_ReadAllPP, readallpp, 8},
	{CmdId_ReadId1, readid1, 8},
	{CmdId_ReadBaud1, readbaud1, 8},
	{CmdId_SetBaud1, setbaud1, 11},
//	{CmdId_ReadId2,readid2, 8},
//	{CmdId_ReadBaud2, readbaud2, 8},
//	{CmdId_SetBaud2, setbaud2, 11},
	{CmdId_RelayQueryStatus, querystatus, 8},
	{CmdId_RelayQueryCoople, querycoople, 8},
	{CmdId_RelayFlashOpen, flashopen, 13},	
	{CmdId_RelayFlashClose, flashclose, 13},
	{CmdId_RelayAllOpen, allopen, 10},
	{CmdId_RelayAllClose, allclose, 10},
};

/* command buffer queue*/
struct commandgrp
{
	struct command combuf[BufferLen]; 	// command buffer for serial thread process
	int step; 					// current command number
	int cnt;					// command count in command buffer
	pthread_mutex_t locker;		// 
	pthread_cond_t ready;
}; 
static struct commandgrp m_commands; // command buffer 
static int toresult = 0;

/* Device old attribute */
static struct termios tty_oldattr;

/* private function defination */
static int check_receive(struct command *cmd, unsigned char *ret, int len);
static int check_receive1(struct command *cmd, unsigned char *ret, int len);
static void deal_commands(int ret);
static void arrange_cmd_queue();
static ssize_t rs485_write(const char *tx_buf, const ssize_t num);
static ssize_t rs485_read(char *rx_buf, const ssize_t num);
static void save_receive(struct command *cmd, unsigned char *ret);
static void save_receive1(struct command *cmd, unsigned char *ret);
static bool is_query_command(int cmdid);
static bool is_query_power(int cmdid);
static bool is_query_relay(int cmdid);

extern sqlite3 *db;

int rs485_init(const char *pathname, const speed_t speed)
{
	struct termios tty_attr;

	/* Open device with RW, NOCTTY, NDELAY */
	if ((fd = open(pathname, O_RDWR | O_NOCTTY)) < 0) {
		fprintf(stderr, "Open %s error on %s\n", pathname, strerror(errno));
		return fd;
	} 

	memset(&tty_attr, 0, sizeof(tty_attr));
	if (tcgetattr(fd, &tty_oldattr)) {
		fprintf(stderr, "Error on getting termios attribute\n");
		close(fd);
		return -1;
	}

	tty_attr.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
	tty_attr.c_cflag |= (speed | CS8 | CLOCAL | CREAD);
	tty_attr.c_iflag |= IGNPAR;
	tty_attr.c_oflag = 0;
	tty_attr.c_lflag = 0;
	tty_attr.c_cc[VMIN]  = 1;
	tty_attr.c_cc[VTIME] = 0;

	tcflush(fd, TCIOFLUSH);

	if (tcsetattr(fd, TCSANOW, &tty_attr)) {
		fprintf(stderr, "Error on setting attrribute\n");
		close(fd);
		return -1;
	}

	/*	if(fcntl(fd, F_SETFL, 0) < 0) {
		perror("Error on setting F_SETFL:");
		close(fd);
		return -1;
	}
	*/
	
	/* Initial command buffer */
	m_commands.step = 0;
	m_commands.cnt = 0;
	//m_commands.locker = PTHREAD_MUTEX_INITIALIZER;
	//m_commands.ready = PTHREAD_COND_INITIALIZER;
	pthread_mutex_init(&m_commands.locker, NULL);
	pthread_cond_init(&m_commands.ready, NULL);

	return fd;
}

ssize_t rs485_write(const char *tx_buf, const ssize_t num)
{
	ssize_t totalLen;
	ssize_t wn;
	ssize_t len;

#ifdef DEBUG 
	int i;
	fprintf(stderr, "=====send data======\n");
	for(i = 0; i < num; i++)
	{
		fprintf(stderr, "send%d=0x%x\n", i, *(tx_buf + i));
	}
#endif

	len = num;
	totalLen = 0;

	do{
		wn = write(fd, tx_buf + totalLen, len - totalLen);
		totalLen += wn;
	} while (totalLen < len);

	return totalLen;
}

ssize_t rs485_read(char *rx_buf, const ssize_t num)
{
	ssize_t res = 0;
	int cycles = 0;

	while (res < 1)
	{
		if (ioctl(fd, FIONREAD, &res) < 0)
		{
			fprintf(stderr, "error on get bytes number\n");
			return Comm_Err_GetByteNum;
		}
		else
		{
			cycles++;
			if (cycles > ReadCycles)
			{
				return Comm_Err_Over_Time;
			}
		}
	}
	
	res = ((res > num) ? num : res);
	if ((res = read(fd, rx_buf, res)) == -1)
	{
		fprintf(stderr, "read data error!\n");
		return Comm_Err_Receive;
	}
	
	return res;
}

void rs485_exit(void)
{
	/* Restitute device attribute */
	if (tcsetattr(fd, TCSANOW, &tty_oldattr) == -1) {
		perror("Restitute error:");
	}

	/* Close the file description of device */
	close(fd);

	/* free mutex locker */
	pthread_mutex_destroy(&m_commands.locker);
	pthread_cond_destroy(&m_commands.ready);
}

unsigned short rs485_calcrc(const char *buf, ssize_t num)
{
    static const unsigned short crc_table[256] = {
		0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
		0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
		0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
		0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
		0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
		0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
		0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
		0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
		0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
		0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
		0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
		0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
		0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
		0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
		0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
		0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
		0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
		0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
		0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
		0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
		0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
		0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
		0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
		0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
		0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
		0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
		0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
		0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
		0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
		0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
		0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
		0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
	};

	unsigned short  crc=0xFFFF;
	unsigned char i;
	const unsigned char *ptr = (const unsigned char *)buf;
	while(num-- > 0)
	{
		i = crc >> 8;
		crc <<= 8;
		crc ^= crc_table[i ^ *ptr++];
	}

	return crc;
}

int check_receive(struct command *cmd, unsigned char *ret, int len)
{
	if (ret == NULL || len < 3)
		return Comm_Err_Empty;
	
//	unsigned short crc  = rs485_calcrc(ret, len -2);
//	unsigned short retcrc = ret[len - 2] * 256 + ret[len - 1];
//	fprintf(stderr, "crc=%d, retcrc=%d\n", crc, retcrc);
//	if (crc != retcrc)
//		return Comm_Err_Data_Check;

	switch (cmd->id)
	{
	case CmdId_ReadAllPP:
		if (ret[2] < 84)
			return Comm_Err_AckFrameLength; 
		break;
	case CmdId_RelayQueryStatus:
		if (ret[2] < 1)
			return Comm_Err_AckFrameLength;
		break;
	default:
		break;
	}
	return Comm_Err_No;
}

void save_receive(struct command *cmd, unsigned char *ret)
{
	int cnt = (int)(ret[2]);
	char sql[256];
	int vol, cur;
	int way = 0;
	bool readway = FALSE;
	int rc = SQLITE_OK;
	char *errmsg = 0;
	int i;

	switch (cmd->id)
	{
		case CmdId_Read1Way:
			way = 1;
			readway = TRUE;	
			break;
		case CmdId_Read2Way:
			way = 2;
			readway = TRUE;	
			break;
		case CmdId_Read3Way:
			way = 3;
			readway = TRUE;	
			break;
		case CmdId_Read4Way:
			way = 4;
			readway = TRUE;	
			break;
		case CmdId_Read5Way:
			way = 5;
			readway = TRUE;	
			break;
		case CmdId_Read6Way:
			way = 6;
			readway = TRUE;	
			break;
		case CmdId_ReadVol:
			if (cnt > 1)
			{
				vol = ret[3] * 256 + ret[4];
				memset(sql, 0, 256);
				sprintf(sql, "update power set volt='%d' where waynum='vr'", vol);
				rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
			}
			break;
		case CmdId_ReadCur:
			if (cnt > 1) 
			{
				cur = ret[3] * 256 + ret[4];
				memset(sql, 0, 256);
				sprintf(sql, "update power set curr='%d' where waynum='cr'", cur);
				rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
			}
			break;
		case CmdId_ReadAllPP:
			fprintf(stderr, "cmdid = %d, cnt = %d\n", cmd->id, cnt); 
			if (cnt > 83)
			{
				for (i = 0; i < CURWAY; i++)
				{
					memset(sql, 0, 256);
					vol = (ret[3 + i * 14] * 256 + ret[4 + i * 14]) / 100;
					cur = (ret[5 + i * 14] * 256 + ret[6 + i * 14]) / 1000;
					sprintf(sql, "update power set volt='%d', curr='%d' where waynum='%d'", vol, cur, i + 1);
					rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
					fprintf(stderr, "sql:%s\n", sql);
				}
			}
			break;
		case CmdId_RelayQueryStatus:
			{
				unsigned char st;
				unsigned char mask = 0x1;
				int tmp;
				if (cnt > 0)
				{
					st = (unsigned char)ret[3];
					for (i = 0; i < 8; i++)
					{
						memset(sql, 0, 256);
						tmp = (st >> i) & mask;
						sprintf(sql, "update power set switch='%d' where waynum = '%d'", tmp, i + 1);
						rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
				//		fprintf(stderr, "sql:%s\n", sql);
					}
				}
			}
			
			break;
		case CmdId_RelayQueryCoople:
			break;
		case CmdId_ReadId1:
			break;
		case CmdId_ReadBaud1:
			break;
		default:
			break;
	}
	
	if (readway)
	{
		if (cnt > 3)
		{
			vol = ret[3] * 256 + ret[4];
			cur = ret[5] * 256 + ret[6];
			vol /=100;
			cur /=1000;
		}
		memset(sql, 0, 256);
		sprintf(sql, "update power set volt='%d', curr='%d' where waynum='%d'", vol, cur, way);
		rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
	}
	
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "sql error:%s\n", errmsg);
		sqlite3_free(errmsg);
		return;
	}
	
}

int check_receive1(struct command *cmd, unsigned char *ret, int len)
{
	if (ret == NULL || len < 3)
		return Comm_Err_Empty;
	
//	unsigned short crc  = rs485_calcrc(ret, len -2);
//	unsigned short retcrc = ret[len - 2] * 256 + ret[len - 1];
//	fprintf(stderr, "crc=%d, retcrc=%d\n", crc, retcrc);
//	if (crc != retcrc)
//		return Comm_Err_Data_Check;

	if (ret[0] == 0x01)
		if (ret[2] < 84)
			return Comm_Err_AckFrameLength;
	else if (ret[0] == 0x02)
		if (ret[2] < 1)
			return Comm_Err_AckFrameLength;
	else
		return Comm_Err_No;

}

void save_receive1(struct command *cmd, unsigned char *ret)
{
	int cnt = (int)(ret[2]);
	char sql[256];
	int vol, cur, ap, total, pf, hz;
	int way = 0;
	bool readway = FALSE;
	int rc = SQLITE_OK;
	char *errmsg = 0;
	int i;
	unsigned char st;
	unsigned char mask = 0x1;
	int tmp;
	char dd[32] = {0};
	char dt[32] = {0};
	
	if (ret[0] == 0x01 && cnt == 84)
	{
		toresult++;
		
		for (i = 0; i < CURWAY; i++)
		{
			memset(sql, 0, 256);
			vol = (ret[3 + i * 14] * 256 + ret[4 + i * 14]) / 100;
			cur = (ret[5 + i * 14] * 256 + ret[6 + i * 14]) / 1000;
			ap = (ret[7 + i * 14] * 256 + ret[8 + i * 14]);
			total = ((ret[9 + i * 14] << 24) + (ret[10 + i * 14] << 16) + (ret[11 + i * 14] << 8) + ret[12 + i * 14] ) / 100;
			pf = (ret[13 + i * 14] * 256 + ret[14 + i * 14]) / 1000;
			hz = (ret[15 + i * 14] * 256 + ret[16 + i * 14]) / 100;
			sprintf(sql, "update power set volt='%d', curr='%d', ap = '%d', total='%d', pf='%d', hz='%d' where waynum='%d'", 
					vol, cur, ap, total, pf, hz, i + 1);
			rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
			// fprintf(stderr, "sql:%s\n", sql);
			
			if (toresult >= 10)
			{
				memset(sql, 0, 256);
				sprintf(sql, "insert into result (waynum, volt, curr, ap, total, pf, hz, date, time) values (\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",%s,%s)", i + 1, vol, cur, ap, total, pf, hz, dd, dt);  
				rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
				toresult = 0;
			}
		}
	}
	else if (ret[0] == 0x2 && ret[1] == 0x1 && cnt == 1)
	{
		st = (unsigned char)ret[3];
		for (i = 0; i < 8; i++)
		{
			memset(sql, 0, 256);
			tmp = (st >> i) & mask;
			sprintf(sql, "update power set switch='%d' where waynum = '%d'", tmp, i + 1);
			rc = sqlite3_exec(db, sql, 0, 0, &errmsg);
			// fprintf(stderr, "sql:%s\n", sql);
		}
	}
		
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "sql error:%s\n", errmsg);
		sqlite3_free(errmsg);
		return;
	}
}

void deal_commands(int ret)
{
	if (ret != Comm_Err_No)
		return;

	pthread_mutex_lock(&m_commands.locker);
	if (m_commands.cnt >  0)
	{
		m_commands.step++;
		if (m_commands.step >= BufferLen)
			m_commands.step = 0;
		m_commands.cnt--;
	}
	else
	{
		m_commands.step = 0;
		m_commands.cnt = 0;
	}
	pthread_mutex_unlock(&m_commands.locker);
}

bool is_query_command(int cmdid)
{
	if (cmdid == CmdId_ReadAllPP || cmdid == CmdId_RelayQueryStatus)
		return TRUE;
	else
		return FALSE; 
}

bool is_query_power(int cmdid)
{
	if (cmdid == CmdId_ReadAllPP)
		return TRUE;
	else
		return FALSE;
}

bool is_query_relay(int cmdid)
{
	if (cmdid == CmdId_RelayQueryStatus)
		return TRUE;
	else
		return FALSE;
}

void arrange_cmd_queue()
{
	int curstep = m_commands.step;
	int nextstep = curstep + 1;
	int queryp = -1;
	int queryl = -1;

	if (m_commands.cnt > 1)
	{
		if (is_query_power(m_commands.combuf[curstep].id))
			queryp = curstep;
		else if (is_query_relay(m_commands.combuf[curstep].id))
			queryl = curstep;
		
		while (nextstep != curstep)
		{
			if (is_query_power(m_commands.combuf[nextstep].id))
			{
				if (queryp == -1)
					queryp = nextstep;
				else
				{
				}
			}		
			else if (is_query_relay(m_commands.combuf[nextstep].id))
			{
				if (queryl == -1)
					queryl = nextstep;
				else
				{
				}
			}
			nextstep++;
			nextstep %= BufferLen;	
		}		
	}
}

int rs485_getcommand(int cmdid, struct command *cmd)
{
	if (cmdid < 0 || cmdid > CMDCOUNT)
		return FALSE;
	
	cmd->id = cmdlist[cmdid].id;
	cmd->proto = cmdlist[cmdid].proto;
	cmd->len = cmdlist[cmdid].len;
	return TRUE;
}

void rs485_sendcommand(int comid)
{
	int pos = 0;
	
	pthread_mutex_lock(&m_commands.locker);
//	while (m_commands.cnt == BufferLen)
//		pthread_cond_wait(&m_commands.ready, &m_commands.locker);
	pos = m_commands.step + m_commands.cnt;
	pos %= BufferLen;
	m_commands.cnt++;
	if (m_commands.cnt > BufferLen)
		m_commands.cnt = BufferLen;
	pthread_mutex_unlock(&m_commands.locker);
	pthread_cond_signal(&m_commands.ready);
	
	m_commands.combuf[pos].id = cmdlist[comid].id;
	m_commands.combuf[pos].proto = cmdlist[comid].proto;
	m_commands.combuf[pos].len = cmdlist[comid].len;

#ifdef DEBUG
	fprintf(stderr, "send:step =%d, cnt = %d\n", m_commands.step, m_commands.cnt);
#endif
}

bool rs485_iscommandempty()
{
	pthread_mutex_lock(&m_commands.locker);
	if (m_commands.cnt == 0)
		return TRUE;
	else
		return FALSE;
	pthread_mutex_unlock(&m_commands.locker);
}

void rs485_run(void *cmd)
{
	int cycles;
	struct command *curcmd = (struct command *)cmd;
	int ret = Comm_Err_No;
	unsigned char recbuf[RecBufLen];

	cycles = SendCycles;
	memset((unsigned char *)recbuf, 0, RecBufLen);
	do
	{
		if (rs485_write(curcmd->proto, curcmd->len) > 0)
		{
			ret = rs485_read(recbuf, RecBufLen);
			if (ret != Comm_Err_Over_Time)
				break;
		}
		
	} while (cycles--);
	
#ifdef DEBUG
	int i;
	fprintf(stderr, "=====send data:id=%d, ret=%d======\n", curcmd->id, ret);
	if (curcmd->id >= CmdId_Open1 && curcmd->id <= CmdId_Close8)
	for(i = 0; i < curcmd->len; i++)
	{
		fprintf(stderr, "send%d=0x%x\n", i, *(curcmd->proto + i));
	}
	 fprintf(stderr, "=====receiver data:cmd=%d, addr=%d======\n", *(recbuf + 1), *(recbuf));
	 for(i = 0; i < ret; i++)
	 {
	   fprintf(stderr, "receive:%d=0x%x\n", i, *(recbuf + i));
	 }
#endif
	 if (ret != Comm_Err_Over_Time ||
		ret != Comm_Err_Receive)
		ret = check_receive1(curcmd, recbuf, ret); 

#ifdef DEBUG
	 fprintf(stderr, "ret=%d, cnt=%d, curcmd.id=%d\n", ret, recbuf[2], curcmd->id);
#endif
	
	// if (ret == Comm_Err_No)
	save_receive1(curcmd, recbuf);

		// deal_commands(ret);
}
