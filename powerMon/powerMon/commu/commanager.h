/**
* @file commanager.h
* @brief 串口通讯管理类定义
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
#ifndef COMMANAGER_H
#define COMMANAGER_H

#include "comthread.h"

class ComManager : public ComThread
{
Q_OBJECT
public:
    explicit ComManager(QObject *parent = 0);

public:
    enum
    {
        ParamStartAddr = 64,
        ParamCnt = 7,
    };

    void waitSendOver();
    void waitSendOverNoLoop();

    /* Com Apps */
    void queryStatus();
    void readDeviceID(quint8 slave = 0x1);
    void readVoltageRange(quint8 slave = 0x1);
    void readCurrentRange(quint8 slave = 0x1);
    void readPowerWayParam(quint16 way = 0, quint8 slave = 0x1);
    void readPowerAllParam(quint16 addr = 0, quint8 slave = 0x1);

    void readCoupleStatus(quint8 slave = 0x2);
    void readRelayStatus(quint8 slave = 0x2);
    void setRelayStatus(int way = 0, int stat = 0, quint8 slave = 0x2);
    void setRelayFlashOpenStatus(quint16 addr = 0, quint16 time = 0, quint8 slave = 0x2);
    void setRelayFlashCloseStatus(quint16 addr = 0, quint16 time = 0, quint8 slave = 0x2);
    void setRelayAllOpenStatus(quint8 slave = 0x2);
    void setRelayAllCloseStatus(quint8 slave = 0x2);

    void readSerialAttribute(quint16 addr = 0, quint8 slave = 0x1);
    void setSerialAttribute(quint16 addr = 0, int baud = 0, quint8 slave = 0x1);

private:
    ComCommand getCommand(int cmdId);
};

#endif // COMMANAGER_H
