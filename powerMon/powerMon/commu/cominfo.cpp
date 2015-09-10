/**
* @file cominfo.cpp
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
#include <QtCore>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>

#include "cominfo.h"
#include "commu/commanager.h"
//#include "message/messageBox.h"
#include "public/statManager.h"
#include "configure/configure.h"
#include "configure/machineconfig.h"
#include "../database/profile.h"

extern ComManager* comManager;

/**
* @brief 构造函数
* @param [I] 父指针
*/
ComInfo::ComInfo(QObject *parent) :
    QObject(parent)
{
    tCount = new QTime();
    tCount->start();

    tQuery = new QTimer(this);
    tQuery->setInterval(QueryTime);

    connect(tQuery, SIGNAL(timeout()), this, SLOT(onQuery()));
    connect(comManager, SIGNAL(comFinish(int, int)),
                    this, SLOT(onComRet(int, int)));
    connect(comManager, SIGNAL(updatePowerParam(int, QByteArray)),
                    this, SLOT(onUpdatePowerParam(int, QByteArray)));
    connect(comManager, SIGNAL(updateDeviceId(int)),
                    this, SLOT(onSetDeviceId(int)));
    connect(comManager, SIGNAL(updateVolRange(int)),
                    this, SLOT(onSetVolRange(int)));
    connect(comManager, SIGNAL(updateCurRange(int)),
                    this, SLOT(onSetCurRange(int)));
    connect(comManager, SIGNAL(updateCoupleStatus(quint8)),
                    this, SLOT(onSetCoupleStatus(quint8)));
    connect(comManager, SIGNAL(updateRelayStatus(quint8)),
                    this, SLOT(onSetRelayStatus(quint8)));

    // 调试用
//    QTimer *dtime = new QTimer(this);
//    dtime->setInterval(5000);
//    dtime->start();
//    connect(dtime, SIGNAL(timeout()), this, SLOT(onDebug()));

}

/**
* @brief 析构函数
*/
ComInfo::~ComInfo()
{
    if (tCount)
        delete tCount;
}

/**
  @brief 定时查询槽函数
*/
void ComInfo::onQuery()
{
    comManager->queryStatus();
}

/**
* @brief 使能定时查询
*/
void ComInfo::enableQuery()
{
    tQuery->start();
}

/**
* @brief 关闭定时查询
*/
void ComInfo::disableQuery()
{
    tQuery->stop();
}

/**
  @brief 通讯返回值处理槽函数
  @param[in] 命令id
  @param[in] 从设备地址
  @param[in] 命令参数
  @param[in] 通讯返回值 0:正常  其它:出错
*/
void ComInfo::onComRet(int id, int ret)
{
    if (ret)
    {
        commu_err_flag = -1;

        //记录通讯错误相关信息,用于提示信息显示
        comErrInfo.errCmdId = id;
        comErrInfo.errorRet = ret;

    //        qDebug() << "com id=" << id << "para=" << para << "ret=" <<ret;

        if (!doneOnce)
        {
            doneOnce = true;
//            MessageBox::reportInfor(Infor_CommuError, 0);
            doneOnce = false;
        }
    }
    else
        commu_err_flag = 0;
}

/**
* @brief 更新测量得到的电力参数
* @param [In] 响应数据
*/
void ComInfo::onUpdatePowerParam(int way, QByteArray ba)
{
#define LENBIT  2
#define DATABIT 3
#define DATALEN 14
    QSqlQuery q(profile.dataDatabase());
    QString sql = "update power set";

    if (way == 0)
    {
        if (ba.count() > 3 && (ba.at(LENBIT) % DATALEN == 0) )
        {
            int cnt = 0;
            int item = 0;
            int way = ba.at(LENBIT) / DATALEN;

            way = (way > MAXTESTITEM ? MAXTESTITEM : way);
            for (int i = 0; i < way; i++)
            {
                cnt = DATABIT +  i * DATALEN;
                pp[item].vol = (ba.at(cnt + 0) * 256 + ba.at(cnt + 1)) / 100;
                pp[item].cur = (ba.at(cnt + 2) * 256 + ba.at(cnt + 3)) / 1000;
                pp[item].ap = ba.at(cnt + 4) * 256 + ba.at(cnt + 5);
                pp[item].total = ((ba.at(cnt + 6) << 24) | (ba.at(cnt + 7) << 16) | (ba.at(cnt + 8) << 8) | (ba.at(cnt + 9))) / 100;
                pp[item].pf = (ba.at(cnt + 10) * 256 + ba.at(cnt + 11)) / 1000;
                pp[item].hz = (ba.at(cnt + 12) * 256 + ba.at(cnt + 13)) / 100;

                sql += QString(" volt='%1',").arg(pp[item].vol);
                sql += QString(" curr='%1',").arg(pp[item].cur);
                sql += QString(" ap='%1',").arg(pp[item].ap);
                sql += QString(" total='%1',").arg(pp[item].total);
                sql += QString(" pf='%1',").arg(pp[item].pf);
                sql += QString(" hz='%1'").arg(pp[item].hz);
                sql += QString(" where waynum='%1'").arg(i+ 1);

                q.exec(sql);
                item++;
            }
        }
    }
    else
    {
        pp[way - 1].vol = (ba.at(DATABIT + 0) * 256 + ba.at(DATABIT + 1)) / 100;
        pp[way - 1].cur = (ba.at(DATABIT + 2) * 256 + ba.at(DATABIT + 3)) /1000;

        sql += QString(" volt='%1',").arg(pp[way - 1].vol);
        sql += QString(" curr='%1'").arg(pp[way - 1].cur);
        sql += QString(" where waynum='%1'").arg(way);
        q.exec(sql);
    }

    sql.clear();
    sql = "insert into result "
          "(waynum, volt, curr, ap, total, pf, hz, date, time) values "
          "(:waynum, :volt, :curr, :ap, :total, :pf, :hz, :date, :time)";
    QDateTime dt = QDateTime::currentDateTime();

    q.prepare(sql);
    for (int i = 0; i < MAXTESTITEM; i++)
    {
        q.prepare(sql);
        q.bindValue(":waynum", QString("%1").arg(i + 1));
        q.bindValue(":volt", pp[i].vol);
        q.bindValue(":curr", pp[i].cur);
        q.bindValue(":ap", pp[i].ap);
        q.bindValue(":total", pp[i].total);
        q.bindValue(":pf", pp[i].pf);
        q.bindValue(":hz", pp[i].hz);
        q.bindValue(":date", dt.date().toString("yyyy-MM-dd"));
        q.bindValue(":time", dt.time().toString("hh:mm:ss"));
        q.exec();
    }

    emit powerParamChanged();
}

/**
* @brief 更新设备id
* @param [In] id
*/
void ComInfo::onSetDeviceId(int id)
{
    deviceId = id;
    emit deviceIdChanged();
}

/**
* @brief 设置电压范围槽函数
* @param [I] 范围
*/
void ComInfo::onSetVolRange(int range)
{
    QSqlQuery q(profile.dataDatabase());
    QString sql = "update power set";
    sql += QString(" volt='%1' where waynum='9'").arg(range);
    q.exec(sql);

    volRange = range;
     emit volRangeChanged();
}

/**
* @brief 设置电范围槽函数
* @param [I] 范围
*/
void ComInfo::onSetCurRange(int range)
{
    QSqlQuery q(profile.dataDatabase());
    QString sql = "update power set";
    sql += QString(" curr='%1' where waynum='10'").arg(range);
    q.exec(sql);

    curRange = range;
    emit curRangeChanged();
}

/**
* @brief 获取串口属性槽函数
* @param [I] 波特率
*/
void ComInfo::onSaveSerialAttribute(int baud)
{
    this->baud = baud;
    emit serialAttributeChanged();
}

/**
* @brief 获取光藕状态槽函数
* @param [I] 状态
*/
void ComInfo::onSetCoupleStatus(quint8 st)
{
    coupleStat = st;
    emit coupleStatusChanged();
}

/**
* @brief 继电器状态槽函数
* @param [I] 状态
*/
void ComInfo::onSetRelayStatus(quint8 st)
{
    relayStat = st;

    quint8 mask = 0x1;
    QSqlQuery q(profile.dataDatabase());
    QString sql = "update power set ";

    for (int i = 0; i < 8; i++)
    {
        int sw = (st & (mask << i));
        sql += QString("switch = '%1' where waynum = '%2'").arg(sw).arg(i + 1);
        q.exec(sql);
    }

    emit relayStatusChanged();
}

/**
* @brief 获取指定路数的电力参数
* @param [I] 路数
* @param [I] 参数
* @retval
*/
int ComInfo::getPowerParam(int i, PowParam &para) const
{
    if (i < 0 || i > MAXTESTITEM)
        return -1;

    para.vol = pp[i].vol;
    para.cur = pp[i].cur;
    para.ap = pp[i].ap;
    para.total = pp[i].total;
    para.pf = pp[i].pf;
    para.hz = pp[i].hz;

    return i;
}

void ComInfo::onDebug()
{
    static int v;

    QSqlQuery q(profile.dataDatabase());
    for(int i = 0; i < 8; i ++)
    {
        pp[i].vol = v;
        pp[i].cur = v + 1;
        pp[i].ap = v + 2;
        pp[i].total = v;
        pp[i].pf = v + 3;
        pp[i].hz = 50;

        QString sql = "update power set";
        sql += QString(" volt='%1',").arg(pp[i].vol);
        sql += QString(" curr='%1',").arg(pp[i].cur);
        sql += QString(" ap='%1',").arg(pp[i].ap);
        sql += QString(" total='%1',").arg(pp[i].total);
        sql += QString(" pf='%1',").arg(pp[i].pf);
        sql += QString(" hz='%1'").arg(pp[i].hz);
        sql += QString(" where waynum='%1'").arg(i+1);

        qDebug() << sql;
        q.exec(sql);
    }
    v++;
    emit powerParamChanged();
}
