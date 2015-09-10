/**
* @file comthread.h
* @brief 串口通讯线程类定义
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
#ifndef COMTHREAD_H
#define COMTHREAD_H

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QTime>

class QByteArray;

/** @brief 通讯协议定义 */
enum
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
    CmdID_QueryStatus,
    CMDCOUNT,
};

/** @brief modbus 控制字 */
enum
{
    MB_ReadRwBit = 0x01,        /**< 读线圈命令 */
    MB_ReadDiscBit = 0x02,      /**< 读离散量状态命令 */
    MB_ReadRwWord = 0x03,       /**< 读取字命令 */
    MB_WriteRwWord = 0x05,      /**< 写控制字命令 */
    MB_WriteMultiBit = 0xF,     /**< 写多个位命令 */
    MB_WriteMultiWord = 0x10,   /**< 写多个字命令 */
};

/**< 通讯异常定义 */
enum
{
    Comm_Err_No = 0,                /**< 成功 */
    Comm_Err_Data,                  /**< 数据错误 */
    Comm_Err_Data_Check,            /**< 校验错误 */
    Comm_Err_Over_Time,             /**< 超时错误 */
    Comm_Err_Read,                  /**< 读缓存错误 */
    Comm_Err_Receive,               /**< 接收错误 */
    Comm_Err_Empty,                 /**< 接收空字节错误 */
    Comm_Err_AckIllegalFunc,        /**< 无效指令 */
    Comm_Err_AckFrameLength,        /**< 数据长度错误 */
    Comm_Err_AckType,               /**< 应答类型错误 */
    Comm_Err_Acknowleget,           /**< 应答 */
};

/**
  @brief 通讯命令结构
  @ingroup commu
*/
class ComCommand
{
public:
    /** @brief 构造函数 */
    ComCommand() : data(0)
    {
        id = 0;
    }

    /** @brief 赋值操作符重载 */
    ComCommand& operator = (const ComCommand& cmd)
   {
        id = cmd.id;
        data = cmd.data;
        return *this;
    }

    int id;             /**< 命令ID */
    QByteArray data;    /**< 命令数据 */
};

/**
  @brief 通讯命令组结构
  @ingroup commu
*/
class CommandGroup
{
public:
    /** @brief 构造函数 */
    CommandGroup() { step = 0; }

    /** @brief 获取当前命令 */
    const ComCommand& curCmd() const
    {
        return commands.at(step);
    }

    int step;   /**< 当前命令号 */
    QList<ComCommand> commands; /**< 命令串 */
};

class ComThread : public QThread
{
Q_OBJECT

public:
    explicit ComThread(QObject *parent = 0);
    ~ComThread();

signals:
    void comFinish(int, int);
    void updatePowerParam(int, QByteArray);
    void updateDeviceId(int);
    void updateVolRange(int);
    void updateCurRange(int);
    void updateSerialAttribute(int);
    void updateCoupleStatus(quint8 stat);
    void updateRelayStatus(quint8 stat);

public slots:

protected:
    void run();
    bool isCommandEmpty();
    void sendCommand(CommandGroup cmd);

    ComCommand readRWBitCommand(quint16 addr, quint16 cnt, quint8 slave = 0x1);
    ComCommand readDiscBitCommand(quint16 addr, quint16 cnt, quint8 slave = 0x1);
    ComCommand readRWWordCommand(quint16 addr, quint16 cnt, quint8 slave = 0x1);
    ComCommand writeRwWordCommand(quint16 addr, quint16 ctl, quint8 slave = 0x1);
    ComCommand writeMultiRWBitCommand(quint16 addr, QByteArray &para, quint8 slave = 0x1);
    ComCommand writeMultiRWWordCommand(quint16 addr, QByteArray &para, quint8 slave = 0x1);

private:
    void _initComm(void);
    void reset(void);
    void _WriteData(const QByteArray&);
    int _ReadData(QByteArray &ba);
    int _CheckReceivedData(const ComCommand& cmd, const QByteArray& Data);
    void signalInfo(const ComCommand& cur_cmd, const QByteArray& ret);
    void arrangeCmdQueue();
    void waitOverTimeOpera();
    void dealCommands(int ret, const QByteArray& revbuf);
    QByteArray dispatchCommand(ComCommand cmd);
    void appendPara(QByteArray& ba);
    unsigned short calCRC(const QByteArray& ba);
    bool isWaitEndCommand(const ComCommand& cmd);
    bool isPairCommand(const CommandGroup& cmd1, const CommandGroup& cmd2);
    bool isQueryStatusCommand(const CommandGroup &cmd);
    bool isMoveFinish(const QByteArray& ret);
    bool nextCmdIsWaitEndCmd(const CommandGroup& group);

    QByteArray int16ToByteArray(quint16 data);

private:
    int m_nFd;                      /**< 串口文件描述符 */
    QList<CommandGroup> m_Commands; /**< 发送命令队列 */
    QMutex mutexCommand;            /**< 命令队列互斥量 */
    QWaitCondition condCom;         /**< 通讯线程等待条件 */
    QWaitCondition waitFinish;      /**< 查询动作完成等待条件 */

    int waitOverCount;              /**< 等待动作完成计数器 */

};

#endif // COMTHREAD_H
