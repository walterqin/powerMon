/**
* @file comthread.cpp
* @brief 串口通讯线程类实现
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <QTime>
#include <QDebug>

#include "comthread.h"
//#include "para/paramManager.h"
//#include "check/contiRunDialog.h"
#include "configure/configure.h"

namespace {
    const int m_nDelay = 200; /**< 发送超时时间 */
    const int m_nCycles = 30; /**< 重试次数 */
    const int m_nWait = 20;    /**< 动作完成查询发送延时 */
    const int m_nWaitOver = 250; /**< 动作完成查询超时 */
}

/**
* @brief 构造函数
* @param　[I] 父指针
*/
ComThread::ComThread(QObject *parent) :
    QThread(parent)
{
    waitOverCount = 0;
    m_Commands.clear();

#ifdef COMTHREAD
    _initComm();
#endif
    start();
}

/**
* @brief 析构函数
*/
ComThread::~ComThread()
{
#ifdef COMTHREAD
    close(m_nFd);
#endif
    wait();
}

/**
* @brief 线程函数
*/
void ComThread::run()
{
    ComCommand cur_command;
    QByteArray send_buf, rev_buf;
    int cycles;

    forever
    {
        mutexCommand.lock();
        while (m_Commands.isEmpty())
        {
//            qDebug()<<"com thread wait";
            condCom.wait(&mutexCommand);
//            qDebug()<<"com thread wake";
        }

        if (m_Commands.at(0).step >= m_Commands.at(0).commands.count() ||
            m_Commands[0].step < 0)
        {
                m_Commands[0].step = 0;
        }

        cur_command = m_Commands[0].curCmd();
        mutexCommand.unlock();

        send_buf = dispatchCommand(cur_command);

//        qDebug() << "========send data========";
//        for(int i = 0; i < send_buf.count(); i++)
//            qDebug("receive:rev_buf[%d]=0x%x", i, send_buf[i]);

        cycles = m_nCycles;
        int ret = Comm_Err_No;
        rev_buf.clear();
        do
        {
            _WriteData(send_buf);
            ret = _ReadData(rev_buf);
            if (ret != Comm_Err_Over_Time)
                break;
        } while(cycles--);

        if (ret != Comm_Err_Over_Time)
            ret = _CheckReceivedData(cur_command, rev_buf);

        emit comFinish(cur_command.id, ret);

//        qDebug() << Q_FUNC_INFO << cur_command.id << ret;
//        for (int i = 0; i < rev_buf.count(); i++)
//            qDebug("receive:rev_buf[%d]=0x%x", i, rev_buf[i]);

        if (ret == 0) //通讯成功
            signalInfo(cur_command, rev_buf);

        dealCommands(ret, rev_buf);

    }
}

/**
* @brief 是否已经没有等待发送的命令(查询命令除外)
* @retval true:是 false:否
*/
bool ComThread::isCommandEmpty()
{
    QMutexLocker locker(&mutexCommand);
    for (int i = 0; i < m_Commands.count(); i++)
    {
        if (isQueryStatusCommand(m_Commands.at(i)))
            continue;
        return false;
    }
    return true;
}

/**
* @brief 发送命令
* @param [I] 命令组
*/
void ComThread::sendCommand(CommandGroup cmd)
{
#ifdef COMTHREAD
        mutexCommand.lock();
        m_Commands.append(cmd);
        mutexCommand.unlock();
        condCom.wakeOne();
#else
        Q_UNUSED(cmd);
#endif
}

/**
* @brief 初始化串口
*/
void ComThread::_initComm(void)
{
#ifdef COMTHREAD
    struct termios option;

    m_nFd = open("/dev/ttySAC3", O_RDWR | O_NOCTTY);

    assert(m_nFd > 0);
    bzero(&option, sizeof(option));
    cfsetispeed(&option, B9600);
    cfsetospeed(&option, B9600);
    option.c_cflag = (B9600 | CS8 | CLOCAL | CREAD);
    option.c_iflag = IGNPAR;
    option.c_oflag = 0;
//    option.c_cc[VMIN]=1;
    option.c_cc[VTIME] = 0;
    tcflush(m_nFd, TCIOFLUSH);
    tcsetattr(m_nFd, TCSANOW, &option);

    m_Commands.clear();
#endif
}

/**
* @brief 复位函数
*/
void ComThread::reset(void)
{
#ifdef COMTHREAD
    close(m_nFd);
#endif
    wait();
}

/**
* @brief 串口发送数据
* @param [I] 数据
*/
void ComThread::_WriteData(const QByteArray &ba)
{
    const char *buf;
    int len;
    int num;
    int totalLen;

    if (ba.isEmpty())
        return;
#if 0
    qDebug() << "Send Data Size:" << ba.size();
    for(int i = 0; i < ba.size(); i++)
    {
        qDebug("0X%x,", ba.at(i));
    }
#endif
    buf = ba.data();
    len = ba.size();
    totalLen=0;

    do{
        num = write(m_nFd, buf + totalLen, len - totalLen);
        totalLen += num;
    } while (totalLen < len);
}

/**
* @brief 读取数据
* @param [O] 返回数据
* @return 数据长度
*/
int ComThread::_ReadData(QByteArray &ba)
{
    QTime send_time; /* 数据发送时间，统计延时*/

    const int DefBufLen = 400;
    int res;
    int ret = Comm_Err_No;
    char readBuf[DefBufLen];

    send_time.start();
    while(res < 1)
    {
        if (ioctl(m_nFd, FIONREAD, &res) < 0)
        {
            ret = Comm_Err_Empty;
            return ret;
        }
        else
        {
            if (send_time.elapsed() > m_nDelay)
            {
                //qDebug() << "elapse time = " << send_time.toString("mm:ss.zzz");
                ret = Comm_Err_Over_Time;
                return ret;
            }
        }
    }

    res = ((res > DefBufLen) ? DefBufLen : res);
    if ((res = read(m_nFd, readBuf, res)) == -1)
    {
        ret = Comm_Err_Read;
        return ret;
    }

    QByteArray data(readBuf, res);

#if 0
    qDebug() << "Receive return valude:" << ret;
    qDebug() << "Receive Data Size:" << data.size();
    for(int i = 0; i < data.size(); i++)
    {
        qDebug("0X%x,", data.at(i));
    }
#endif
    if (ret == Comm_Err_No)
        ba = data;

    return ret;
}

/**
* @brief 接收数据校验
* @param [I] 发送的命令
* @param [I] 接收的数据
* @retval
*   */
int ComThread::_CheckReceivedData(const ComCommand& cmd, const QByteArray& Data)
{
    if (Data.isEmpty() || (Data.size() <= 1))
            return Comm_Err_Empty;

    //接收数据校验
    int cnt = Data.size();
    quint16 checkSum = calCRC(Data.left(cnt - 2));
    quint16 recSum = Data.at(cnt - 2) * 256 + Data.at(cnt - 1);

    qDebug() << "send crc=" << checkSum << "rec crc=" << recSum << "rec id=" << cmd.id;
#if 0
    qDebug() << "Receive Data Size:" << Data.size();
    for(int i = 0; i < Data.size(); i++)
    {
        qDebug("0X%x,", Data.at(i));
    }
#endif

    if (checkSum != recSum)
        return Comm_Err_Data_Check;

    //接收错误
    if (cmd.id != Data.at(1))
        return Comm_Err_Receive;

    return Comm_Err_No;
}

/**
* @brief 信号处理
* @param [I] 当前命令
* @param [I] 响应数据
*/
void ComThread::signalInfo(const ComCommand& cur_cmd, const QByteArray& ret)
{
    if (ret.count() < 4)
        return;
    int way;
    bool readway = false;

    switch(cur_cmd.id)
    {
    case CmdId_Read1Way:
         way = 1;
         readway = true;
         break;
    case CmdId_Read2Way:
         way = 2;
         readway = true;
         break;
    case CmdId_Read3Way:
         way = 3;
         readway = true;
         break;
    case CmdId_Read4Way:
        way = 4;
        readway = true;
        break;
    case CmdId_Read5Way:
        way = 5;
        readway = true;
        break;
    case CmdId_Read6Way:
        way = 6;
        readway = true;
        break;
    case CmdId_ReadVol:
        emit updateVolRange((int)((ret.at(3) << 8) | ret.at(4)));
        break;
    case CmdId_ReadCur:
        emit updateCurRange((int)((ret.at(3) << 8) | ret.at(4)));
        break;
    case CmdId_ReadAllPP:
        way = 0;
        readway = true;
        break;
    case CmdId_RelayQueryStatus:
        emit updateRelayStatus(ret.at(3));
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
        emit updatePowerParam(way, ret);
    qDebug() << "readway:" << readway;
}

/**
* @brief 命令解析
* @param [I] 命令
* @retval 协议数据
*/
QByteArray ComThread::dispatchCommand(ComCommand command)
{
    QByteArray ba;
    ba.append(command.data);
    return ba;
}

/**
  @brief 处理命令队列
  @param[in] 当前发送的返回值
  @param[in] 当前发送的返回信息
*/
void ComThread::dealCommands(int ret, const QByteArray& revbuf)
{
    QMutexLocker locker(&mutexCommand);
    if (m_Commands.isEmpty() || m_Commands.at(0).commands.isEmpty())
        return;

    /* 由于目前通讯出错为自恢复错误,出错后仍然会重发原有命令*/
    if (ret)
    {
        return;
    }

    if (isQueryStatusCommand(m_Commands.at(0)))
    {
        m_Commands[0].step++;
        if (m_Commands.at(0).step >= m_Commands.at(0).commands.count())
            m_Commands.removeFirst();
        return;
    }

    //查询动作完成超时，删除当前命令，并执行状态查询命令
    if (waitOverCount >= m_nWaitOver)
    {
        waitOverCount = 0;
        waitOverTimeOpera();
        return;
    }

    if (isWaitEndCommand(m_Commands.at(0).curCmd())) //需要等待动作是否完成的命令
    {
        if (isMoveFinish(revbuf))
        {
            m_Commands[0].step++;
            waitOverCount = 0;
        }
        else
        {
            waitFinish.wait(&mutexCommand, m_nWait);
            waitOverCount++;
        }
    }
//    else if () //TODO:其他的需要判断的条件
//    {
//
//    }
    else
    {
        if (nextCmdIsWaitEndCmd(m_Commands.at(0)))
        {
            waitFinish.wait(&mutexCommand, m_nWait);
            waitOverCount = 0;
        }
        m_Commands[0].step++;
    }

    if (m_Commands.at(0).step >= m_Commands.at(0).commands.count())
        m_Commands[0].step = -1;

    arrangeCmdQueue();

    if (m_Commands.at(0).step == -1)
        m_Commands.removeFirst();
}

void ComThread::appendPara(QByteArray& ba)
{
    Q_UNUSED(ba)
}

/**
* @brief 计算CRC校验值
* @param[in] 桢数据
* @return crc值
*/
unsigned short ComThread::calCRC(const QByteArray& ba)
{
    unsigned short crc = (unsigned short)0xffff;
    for (int len = 0; len < ba.count(); len++)
    {
        crc ^= (ba.at(len) << 8);
        for (int i = 0; i < 8; i++)
        {
            if (crc & 0x8000)
            {
                crc <<= 1;
                crc ^= 0x1021;
            }
            else
                crc <<= 1;
        }
    }
    return crc;
}

/**
  @brief 是否是需要等待结束的命令
  @param[in] 命令
  @retval true:是 false:否
*/
bool ComThread::isWaitEndCommand(const ComCommand& cmd)
{
    Q_UNUSED(cmd)
    return false;
}

/**
* @brief 判断2个命令是否是配对命令
* @param[in] 命令1
* @param[in] 命令2
* @retval true:是 false:否
*/
bool ComThread::isPairCommand(const CommandGroup& cmd1, const CommandGroup& cmd2)
{
    if (cmd1.commands.isEmpty() || cmd2.commands.isEmpty())
        return false;

    int pair_type=0;
    for (int i = 0; i < cmd1.commands.count(); i++)
    {
        //TODO:根据条件给pair_type赋值
//        pair_type =
    }

    if (pair_type)
    {
        for (int i = 0; i < cmd2.commands.count(); i++)
        {
            //TODO:根据pair_type的值确定
            return true;
        }
    }
    return false;
}

/**
* @brief 整理消息队列
*/
void ComThread::arrangeCmdQueue()
{
    if (m_Commands.count() > 1)
    {
        if (m_Commands.at(0).commands.isEmpty())
            return;

        bool a_pair = false;
        bool one_query = false;

        int count;
        for (int i = 1; i < (count = m_Commands.count()); i++)
        {
            /* 只保留队列中第一个查询命令 */
            if (isQueryStatusCommand(m_Commands.at(i)))
            {
                if (!one_query)
                    one_query = true;
                else
                {
                    m_Commands.removeAt(i);
                    i--;
                    count--;
                }
            }
//            else if () //TODO: 其他需要判断的条件
//            {
//
//            }
            else
            {
                if (!a_pair)
                {
                   if (!isPairCommand(m_Commands.at(0), m_Commands.at(i)))
                   {
                       m_Commands.removeAt(i);
                       i--;
                       count --;
                   }
                   else
                       a_pair = true;
                }
                else
                {
                    m_Commands.removeAt(i);
                    i--;
                    count --;
                }
            }
        }
    }
}

/**
* @brief 查询动作完成等待超时处理
*
* 超时后删除掉当前命令组，执行队列中的查询状态命令
*/
void ComThread::waitOverTimeOpera()
{
    if (m_Commands.isEmpty())
        return;

    m_Commands.removeFirst();
    for (int i = 0; i < m_Commands.count(); i++)
    {
        if (isQueryStatusCommand(m_Commands.at(i)))
        {
            m_Commands.move(i, 0);
            break;
        }
    }
}

/**
*  @brief 是否是定时查询命令
*  @param[in] 命令
*  @retval true:是 false:否
*/
bool ComThread::isQueryStatusCommand(const CommandGroup &cmd)
{
    if (cmd.commands.isEmpty())
        return false;

    if (cmd.commands.at(0).id == CmdID_QueryStatus)
        return true;

    return false;
}

/**
  @brief 动作是否已经结束
  @param[in] 通讯返回信息
  @retval true:是 false:否
*/
bool ComThread::isMoveFinish(const QByteArray& ret)
{
    if (ret.size() > 3)
    {
        if (ret.at(3) == (char)0x0)
            return true;
    }

    return false;
}

/**
* @brief 判断当前命令组下一个命令是否为动作完成查询命令
* @param[in] 命令组
* @retval true:是 false:否
*/
bool ComThread::nextCmdIsWaitEndCmd(const CommandGroup& group)
{
    Q_UNUSED(group)
    return false;
}

/**
* @brief 读下位机的开关状态
* @param [In] 地址
* @param [In] 数量
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::readRWBitCommand(quint16 addr, quint16 cnt, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(cnt)
    Q_UNUSED(slave)
    ComCommand cmd;
    return cmd;
}

/**
* @brief 读下位机的离散量
* @param [In] 地址
* @param [In] 数量
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::readDiscBitCommand(quint16 addr, quint16 cnt, quint8 slave)
{
    Q_UNUSED(addr)
    Q_UNUSED(cnt)
    Q_UNUSED(slave)
    ComCommand cmd;
    return cmd;
}

/**
* @brief 读字
* @param [In] 地址
* @param [In] 数量
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::readRWWordCommand(quint16 addr, quint16 cnt, quint8 slave)
{
    ComCommand cmd;

    Q_UNUSED(addr)
    Q_UNUSED(cnt)
    Q_UNUSED(slave)
    return cmd;
}

/**
* @brief 读字
* @param [In] 地址
* @param [In] 控制字
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::writeRwWordCommand(quint16 addr, quint16 ctl, quint8 slave)
{
    ComCommand cmd;

    Q_UNUSED(addr)
    Q_UNUSED(ctl)
    Q_UNUSED(slave)

    return cmd;
}

/**
* @brief 写多个bit命令控制字
* @param [In] 地址
* @param [In] 命令
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::writeMultiRWBitCommand(quint16 addr, QByteArray &para, quint8 slave)
{
    ComCommand cmd;

    Q_UNUSED(addr)
    Q_UNUSED(para)
    Q_UNUSED(slave)

    return cmd;
}

/**
* @brief 写多个命令控制字
* @param [In] 地址
* @param [In] 命令
* @param [In] 从机地址
* @retval 命令
*/
ComCommand ComThread::writeMultiRWWordCommand(quint16 addr, QByteArray &para, quint8 slave)
{
    ComCommand cmd;

    Q_UNUSED(addr)
    Q_UNUSED(para)
    Q_UNUSED(slave)

    return cmd;
}

/**
* @brief 将一个short型的数据专程qbytearray保存，且高字节在前
* @param [I] 数据
* @retval
*   QByteArray
*/
QByteArray ComThread::int16ToByteArray(quint16 data)
{
    QByteArray ba;
    quint8 tmp;

    ba.clear();
    tmp = (data >> 8) & 0xff;
    ba.append(tmp);
    tmp = data & 0xff;
    ba.append(tmp);

    return ba;
}
