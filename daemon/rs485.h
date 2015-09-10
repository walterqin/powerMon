#ifndef RS485_H
#define RS485_H

#include <termios.h>
#include <linux/serial.h>

#define MAXWAY 8
#define PROLEN 8
#define CURWAY 6

typedef int bool;
#define TRUE 1
#define FALSE 0

/** @brief command index defination */
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
	CmdId_Read1Way,
	CmdId_Read2Way,
	CmdId_Read3Way,
	CmdId_Read4Way,
	CmdId_Read5Way,
	CmdId_Read6Way,
	CmdId_ReadVol,
	CmdId_ReadCur,
	CmdId_ReadAllPP,
	CmdId_ReadId1,
	CmdId_ReadBaud1,
	CmdId_SetBaud1,
//	CmdId_ReadId2,
//	CmdId_ReadBaud2,
//	CmdId_SetBaud2,
	CmdId_RelayQueryStatus,
	CmdId_RelayQueryCoople,
	CmdId_RelayFlashOpen,
	CmdId_RelayFlashClose,
	CmdId_RelayAllOpen,
	CmdId_RelayAllClose,
	CMDCOUNT,
};

/* communication error definition */
enum
{
	Comm_Err_No = 0,
	Comm_Err_Data = -1,
	Comm_Err_Over_Time = -2,
	Comm_Err_Data_Check = -3,
	Comm_Err_Receive = -4,
	Comm_Err_Empty = -5,
	Comm_Err_AckIllegalFunc = -6,
	Comm_Err_AckFrameLength = -7,
	Comm_Err_AckType = -8,
	Comm_Err_Acknowledge = -9,
	Comm_Err_GetByteNum = -10,
};

/* command frame structure */
struct command
{
	int id;
	unsigned char *proto;
	int len;
};

/* Device file description */
extern int fd;

extern int rs485_init(const char *pathname, const speed_t speed);
extern void rs485_exit(void);
extern void rs485_sendcommand(int comid);
extern bool rs485_iscommandempty();
extern void rs485_run(void *cmd);
extern unsigned short rs485_calcrc(const char *buf, ssize_t num);
extern int rs485_getcommand(int cmdid, struct command *cmd);

#endif
