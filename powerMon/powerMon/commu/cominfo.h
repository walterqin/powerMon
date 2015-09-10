/**
* @file cominfo.h
* @brief 串口通讯信息管理类定义
* @author qgj
* @date 2015-06-13
* @ingroup commu
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-13    QinGenjian    create
*
*/
#ifndef COMINFO_H
#define COMINFO_H

#include <QObject>
#include "configure/machineconfig.h"

class QTimer;
class QWidget;
class QTime;
class QByteArray;

/** 通讯错误显示信息 */
typedef struct
{
    int errCmdId;                   /**< 通讯错误时的命令ID */
    int errorRet;                   /**< 通讯返回错误 */
    int addr;                       /**< 设备地址 */
    int status;                     /**< 当前命令字 */
} CommErrInfo;

typedef struct
{
    int vol;    /**< 电压 V */
    int cur;    /**< 电流 A */
    int ap;     /**< 有功功率 W */
    int total;  /**< 有功总电能 kWh */
    int pf;     /**< 功率因数 */
    int hz;     /**< 频率 hz*/
}PowParam;

class ComInfo : public QObject
{
Q_OBJECT
public:
    explicit ComInfo(QObject *parent = 0);
    ~ComInfo();

    /** 状态查询时间定义 */
    enum { QueryTime = 100 };
    void enableQuery();
    void disableQuery();

    int getPowerParam(int i, PowParam &para) const;
    int getVolRange() const { return volRange; }
    int getCurRange() const { return curRange; }
    int getDeviceId() const { return deviceId; }
    int getSerialAttri() const { return baud; }
    int getCoupleStatus() const { return coupleStat; }
    int getRelayStatus() const  { return relayStat; }

signals:
    void powerParamChanged();
    void deviceIdChanged();
    void volRangeChanged();
    void curRangeChanged();
    void serialAttributeChanged();
    void coupleStatusChanged();
    void relayStatusChanged();


public slots:
    void onComRet(int id, int ret);
    void onQuery();
    void onUpdatePowerParam(int way, QByteArray ba);
    void onSetDeviceId(int id);
    void onSetVolRange(int range);
    void onSetCurRange(int range);
    void onSaveSerialAttribute(int baud);
    void onSetCoupleStatus(quint8 st);
    void onSetRelayStatus(quint8 st);
    void onDebug();

private:
    int commu_err_flag;       /**< 通讯出错 */
    int errID;                /**< 错误号 */
    bool doneOnce;            /**< 避免重复操作标志 */

    QTimer *tQuery;           /**< 状态查询定时器 */
    QTime *tCount;            /**< 计时定时器 */

    CommErrInfo comErrInfo;   /**< 通讯错误发生时的信息 */

    PowParam pp[MAXTESTITEM];
    int volRange;
    int curRange;
    int deviceId;
    int baud;
    int coupleStat;
    int relayStat;

};

#endif // COMINFO_H
