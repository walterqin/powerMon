/**
* @file statManager.h
* @brief 计数机制，维修保养，密码，实时时钟校验机制，蜂鸣器，背光，镜像点保存的杂项类
* @author qgj
* @date 2015-06-04
* @ingroup public
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/

#ifndef STATMANAGER_H
#define STATMANAGER_H

#include <QObject>
#include <time.h>
#include <QTime>
#include "pushButton.h"

/**
  @class TCHScreenMgr
  @brief 触摸屏灵敏度管理类
  @ingroup sram
*/
class TCHScreenMgr
{
public:
    static void open();
    static void setSensVal(int sens=5);
};
/**
  @class ChkPointMgr
  @brief 镜像点管理类
  @ingroup sram
*/
class ChkPointMgr
{
public:
    static void saveCheckPoint();
};
/**
  @class BuzzerMgr
  @brief 蜂鸣器管理类
  @ingroup sram
*/
class BuzzerMgr
{
public:
    static void start(unsigned msec=10);
    static void stop();
};
/**
  @class BackLightMgr
  @brief 背光管理类
  @ingroup sram
*/
class BackLightMgr
{
public:
    static void open();
    static void close();
    static void setBackLight(int light);
};
/**
  @class StatManager
  @brief 计数机制，维修保养，密码，实时时钟校验机制的杂项类
  @ingroup sram
*/
class StatManager : public QObject
{
    Q_OBJECT
public:
    explicit StatManager(QObject *parent = 0);

    void panelStyle();

signals:
    void updateCount();         /**< 更新计数器的信号 */

protected:
    void timerEvent(QTimerEvent *);

public:
    void updateFromStopped(int milli);
    void updateFromNormal();
    int saveHWClockTime();
    void saveCurTime();
    bool closeAutoBackLight(QEvent *event);
    void startBuzzer();
    void OpenBackLight(void);
    void checkPassword();

protected:
    void init();    
    void updateMaintainForTime(int milli);
    void checkClock();
    void updateRunNoteForTime(int milli);

private:
    void beginTime();
    void endTime();
    time_t getClockTime();

private:
    QTime *m_Time;   /**< 计时器 */
    bool m_bCloseBL; /**< 背光关闭标志 */
    bool m_bStarted;  /**< 屏保初始计时标志 */
    bool counterOver; /**< 计数器溢出标志 */

};


#endif // MGRSTAT_H
