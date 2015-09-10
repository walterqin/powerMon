/**
* @file commanager.cpp
* @brief 串口通讯管理类实现
* @author qgj
* @date 2015-06-15
* @ingroup commu
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-15    QinGenjian    create
*
*/
#include <QtDebug>
#include <QTime>
#include <QApplication>
#include "commanager.h"
#include "cominfo.h"
#include "configure/machineconfig.h"
#include "configure/configure.h"

#define PROLEN 8
#define MAXWAY 8
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

struct command
{
    int id;
    unsigned char *proto;
    int len;
};
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

/**
* @brief 构造函数
* @param [I] 父指针
*/
ComManager::ComManager(QObject *parent) :
    ComThread(parent)
{

}

/**
* @brief 等待前一个命令发送完毕
* @note 当一个按键需要发送2组命令时，需要在第2组命令发送前加入此函数进行等待。
*/
void ComManager::waitSendOver()
{
#ifdef COMTHREAD
    while (!isCommandEmpty())
    {
        qApp->processEvents();
    }
#endif
}

/**
* @brief 等待前一个命令发送完毕
* @note 当一个按键需要发送2组命令时，需要在第2组命令发送前加入此函数进行等待。
*/
void ComManager::waitSendOverNoLoop()
{
#ifdef COMTHREAD
    while (!isCommandEmpty())
    {
    }
#endif
}

/* Com Apps */
void ComManager::queryStatus()
{
#ifdef COMTHREAD
    readPowerAllParam();
    waitSendOver();
    readRelayStatus();
#endif
}

/**
* @brief 读设备id
*/
void ComManager::readDeviceID(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_ReadId1);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 读取电压量程
* @param [I] 从设备
*/
void ComManager::readVoltageRange(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_ReadVol);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 读取电流量程
* @param [I] 从设备
*/
void ComManager::readCurrentRange(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_ReadCur);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 读取指定路号的电力参数
* @param [I] 路号，从1开始
* @param [I] 从设备地址
*/
void ComManager::readPowerWayParam(quint16 way, quint8 slave)
{
    if (way < 1 || way > 6)
        return;

    Q_UNUSED(way)
    Q_UNUSED(slave)

#ifdef COMTHREAD
    int cmdId = CmdId_Read1Way + way - 1;

    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(cmdId);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

void ComManager::readPowerAllParam(quint16 addr, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_ReadAllPP);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 获取光耦状态
*/
void ComManager::readCoupleStatus(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayQueryCoople);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 获取继电器状态
*/
void ComManager::readRelayStatus(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayQueryStatus);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 设定指定路号的继电器状态
* @param [I] 路号
* @param [I] 状态 0x0000 关闭 0xFF00打开
* @param [I] 从设备
*/
void ComManager::setRelayStatus(int way, int stat, quint8 slave)
{
    if (way > 6 || way < 1 || stat > 1 || stat < 0)
        return;

    Q_UNUSED(way)
    Q_UNUSED(stat)
    Q_UNUSED(slave)

#ifdef COMTHREAD
    CommandGroup cmdGrp;
    int cmdId;

    if (stat == 0)
        cmdId = CmdId_Close1 + way - 1;
    else
        cmdId = CmdId_Open1 + way  - 1;

    ComCommand cmd = getCommand(cmdId);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 继电器闪开设置
* @param [I] 地址
* @param [I] 闪开时间, 精度0.1s
* @param [I] 从设备
*/
void ComManager::setRelayFlashOpenStatus(quint16 addr, quint16 time, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(time)
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayFlashOpen);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 继电器闪闭设置
* @param [I] 地址
* @param [I] 闪开时间, 精度0.1s
* @param [I] 从设备
*/
void ComManager::setRelayFlashCloseStatus(quint16 addr, quint16 time, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(time)
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayFlashClose);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 继电器全开
* @param [I] 从设备
*/
void ComManager::setRelayAllOpenStatus(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayAllOpen);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 继电器全闭
* @param [I] 从设备
*/
void ComManager::setRelayAllCloseStatus(quint8 slave)
{
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_RelayAllClose);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 读串口属性
* @param [I] 地址
* @param [I] 从设备
*/
void ComManager::readSerialAttribute(quint16 addr, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_ReadBaud1);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 写串口属性
* @param [I] 地址
* @param [I] 波特率
* @param [I] 从设备
*/
void ComManager::setSerialAttribute(quint16 addr, int baud, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(baud)
    Q_UNUSED(slave)
#ifdef COMTHREAD
    CommandGroup cmdGrp;
    ComCommand cmd = getCommand(CmdId_SetBaud1);
    cmdGrp.commands.append(cmd);
    sendCommand(cmdGrp);
#endif
}

/**
* @brief 获取指定命令的协议数据
* @param [I] 命令id
* @retval
*   命令协议
*/
ComCommand ComManager::getCommand(int cmdId)
{
    struct command cmd;
    ComCommand ret;

    cmd = cmdlist[cmdId];
    ret.id = cmd.id;
    ret.data.append((char *)cmd.proto, cmd.len);

    return ret;
}
