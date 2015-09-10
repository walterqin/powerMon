/**
*  @file statManager.cpp
*  @brief 蜂鸣器，背光，镜像点保存的杂项类
*  @author qgj
*  @date 2015-06-04
*  @ingroup public
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/

#include <QtGui>
#include "statManager.h"
#include "configure/configure.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <unistd.h>


static time_t sysTime_start;    /**< 系统时钟起始计数 */
static time_t clkTime_start;    /**< 硬件时钟起始计数 */
static time_t sysTime_end;      /**< 系统时钟结束计数 */
static time_t clkTime_end;      /**< 硬件时钟结束计数 */
static time_t dure_time_start;  /**< 系统时钟中间变量起始计数 */
static time_t dure_time_end;    /**< 系统时钟中间变量结束计数 */
static time_t power_time_start; /**< 上电起始时间 */

#define BL_MAGIC 'p'            /**< 背光驱动接口需要的幻数 */
#define BL_IOCTL_OPEN 	_IO(BL_MAGIC, 0)     /**< 背光打开 */
#define BL_IOCTL_CLOSE	_IO(BL_MAGIC, 1)     /**< 背光关闭 */
#define BL_IOCTL_SET_FREQ _IOW(BL_MAGIC, 2, unsigned) /**< 背光驱动接口 */

#define PWM_IOCTL_OPEN 1        /**< 蜂鸣器打开 */
#define PWM_IOCTL_CLOSE 0       /**< 蜂鸣器关闭 */

#define INPUT_IOC_MAGIC 'i'     /**< 触摸屏驱动接口需要的幻数 */
#define INPUT_IOCSETSENS _IOW(INPUT_IOC_MAGIC, 1, int) /**< 触摸屏驱动接口 */

namespace {
    const int HourForMs = 3600000; /**< 对于ms的小时计数 */
}

/**
  @brief 设置触摸屏的灵敏度
  @param[in] sens : 1-5 灵敏度的灵敏程度，制越大越灵敏
*/
void TCHScreenMgr::setSensVal(int sens)
{
#ifndef X86_COMPILE
    int fd = ::open("/dev/input_controle",O_WRONLY);
    if (sens < 1)
        sens = 1;
    else if(sens > 5)
        sens = 5;
    ioctl(fd, INPUT_IOCSETSENS, sens);
    close(fd);
#else
    sens = sens;
#endif
}

/**
  @brief 按设定值设定灵敏度
*/
void TCHScreenMgr::open()
{
#ifndef X86_COMPILE
    //unsigned sens = paramManager->getPanelAddr()->Key_Sensi;
    //setSensVal(sens);
#endif
}

/**
  @brief 关闭背光，保存镜像点
*/
void ChkPointMgr::saveCheckPoint()
{
    sync();
}

/**
  @brief 启动蜂鸣器
  @param[in] msec 蜂鸣器鸣响持续时间
*/
void BuzzerMgr::start(unsigned msec)
{
#ifndef X86_COMPILE
    int fd = open("/dev/pwm",O_WRONLY);
    if(msec>=10000)
        msec = 10000;
    else if(msec==0)
        msec = 1;
    ioctl(fd,PWM_IOCTL_OPEN,msec);
    close(fd);
#else
    msec = msec;
#endif
}

/**
  @brief 停止蜂鸣器
*/
void BuzzerMgr::stop()
{
#ifndef X86_COMPILE
    int fd = ::open("/dev/pwm",O_WRONLY);
    ioctl(fd,PWM_IOCTL_CLOSE);
    close(fd);
#endif
}

/**
  @brief 调节背光亮度
  @param[in] 亮度
*/
void BackLightMgr::setBackLight(int light)
{
#ifndef X86_COMPILE
    unsigned freq = light;
    int fd = ::open("/dev/backlight",O_WRONLY);
    freq--;
    if(freq>100)
        freq = 100;
    else if(freq<4)
        freq=4;
    ioctl(fd,BL_IOCTL_SET_FREQ,freq);
    ::close(fd);
#else
    light = light;
#endif
}

/**
  @brief 打开背光
*/
void BackLightMgr::open()
{
#ifndef X86_COMPILE
    int fd = ::open("/dev/backlight",O_WRONLY);
    ioctl(fd,BL_IOCTL_OPEN);
    ::close(fd);
#endif
}

/**
  @brief 关闭背光
*/
void BackLightMgr::close()
{
#ifndef X86_COMPILE
    int fd = ::open("/dev/backlight",O_WRONLY);
    ioctl(fd,BL_IOCTL_CLOSE);
    ::close(fd);
#endif
}

/**
  @brief 构造函数
  @param[in] 父指针
*/
StatManager::StatManager(QObject *parent) :
        QObject(parent)
{
#ifdef X86_COMPILE  //   X86_COMPILE
	//startTimer(1000);
#endif
    init();    
    counterOver = false;
}

/**
  @brief 初始化相关信息，获取当前时间
*/
void StatManager::init()
{
    m_bCloseBL = false;
    m_bStarted = false;
        m_Time = new QTime;

    beginTime();
    if(QFile::exists("/Data/setup/lastTime"))
    {
        QFile file("/Data/setup/lastTime");
        file.open(QIODevice::ReadOnly);
        QByteArray ba = file.readAll();
        QString str(ba);
        unsigned int saveTime = str.toUInt();
        if((unsigned int)dure_time_start<saveTime)
        {
            struct timeval tv;
            tv.tv_sec=saveTime;
            tv.tv_usec=0;
            settimeofday(&tv,NULL);
            StatManager::saveHWClockTime();
        }
    }
}

/**
  @brief 检查硬件时钟是否工作正常
*/
void StatManager::checkClock()
{
    endTime();

    int dure_time = dure_time_end-dure_time_start;
    int saveTime;

    if(dure_time>=3600)
    {
        char buf[100];
        dure_time_start=dure_time_end;
        if(QFile::exists("/Data/setup/lastTime"))
        {
            QFile file("/Data/setup/lastTime");
            file.open(QIODevice::ReadOnly);
            QByteArray ba = file.readAll();
            QString str(ba);
            saveTime = str.toInt();
            if(dure_time_start>saveTime)
            {
                sprintf(buf,"/bin/echo %d > /Data/setup/lastTime",(int)dure_time_start);
                system(buf);
            }
        }
        else
        {
            sprintf(buf,"/bin/echo %d > /Data/setup/lastTime",(int)dure_time_start);
            system(buf);
        }
    }
    int deltaClkTime = clkTime_end-clkTime_start;
    int deltaSysTime = sysTime_end-sysTime_start;
    if(deltaSysTime>1000)
    {
        deltaClkTime = clkTime_end-clkTime_start;
        if(abs(deltaClkTime-deltaSysTime)>240)
        {
//            qDebug()<<"----------------start sys time = " <<sysTime_start;
//            qDebug()<<"----------------start clock time = "<<clkTime_start;
//            qDebug()<<"----------------end sys time = " <<sysTime_end;
//            qDebug()<<"----------------end clock time = "<<clkTime_end;
//            qDebug()<<"----------------delta sys time = " <<deltaSysTime;
//            qDebug()<<"----------------delta clock time = "<<deltaClkTime;
//			messageBox::reportInfor(Infor_HWClockFailed);
        }
    }
}

/**
  @brief 检查用户收费密码是否到期
  @param[in] 初始化调用标志
*/
void StatManager::checkPassword()
{
//	if (passwordMng::isSavedPw())
//    {
//		if (passwordMng::curPasswordExpire())
//        {
//			passwordExpire pwd;
//            pwd.exec();
//        }
//    }
}

/**
  @brief 供周期调用的接口，用来检查密码到期和硬件时钟
*/
void StatManager::updateFromNormal()
{
    checkClock();
    checkPassword();
}

/**
  @brief 提取当前系统时间和硬件时钟时钟时间并保存，以秒为单位
*/
void StatManager::beginTime()
{
    struct timeval tv;
    dure_time_start = clkTime_start = getClockTime();
    gettimeofday(&tv,NULL);
    sysTime_start = tv.tv_sec;
    power_time_start = clkTime_start;
}

/**
  @brief 获取当前时间值
*/
void StatManager::endTime()
{
    struct timeval tv;
    dure_time_end = clkTime_end = getClockTime();
    gettimeofday(&tv,NULL);
    sysTime_end = tv.tv_sec;
}

/**
  @brief 获取硬件时间的当前值，返回值是秒数
*/
time_t StatManager::getClockTime()
{
    struct tm mytm;
    int fd = open("/dev/rtc",O_RDONLY);
    ioctl(fd,RTC_RD_TIME,&mytm);
    close(fd);
    return mktime(&mytm);
}

/**
  @brief 将当前系统时间保存到实时时钟
  @retval       0 : 成功,other : 失败
*/
int StatManager::saveHWClockTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    struct tm curTm;
    curTm = *localtime(&tv.tv_sec);
    int fd = open("/dev/rtc",O_WRONLY);
    int ret = ioctl(fd,RTC_SET_TIME,&curTm);
    close(fd);

    if(m_bStarted)
        m_Time->restart();

    return ret;
}

/**
  @brief 将当前时间保存到指定的文件中
*/
void StatManager::saveCurTime()
{
    char buf[100];
    struct tm mytm;
    int fd = open("/dev/rtc",O_RDONLY);
    ioctl(fd,RTC_RD_TIME,&mytm);
    close(fd);
    int t1 = mktime(&mytm);
    int t2 = time(NULL);
    int t = t1;
    if(t1<t2)
        t = t2;
    sprintf(buf,"/bin/echo %d > /Data/setup/lastTime",(unsigned int)t);
    system(buf);
}

/**
  @brief 蜂鸣器鸣叫
*/
void StatManager::startBuzzer()
{
    BuzzerMgr::start();
}

/**
  @brief 自动关闭背光
  @param[in] event 事件类型
  @retval true--已经处理，不需要进一步处理 @n
          false 没有处理，需进一步处理
*/
bool StatManager::closeAutoBackLight(QEvent *event)
{
    event = event;
//	if(paramManager->getPanelAddr()->Back_Light_Switch == 1)
//    {
//        if(!m_bStarted)
//        {
//            m_bStarted = true;
//            m_Time->start();
//        }
//        if((event->type()==QEvent::MouseButtonPress))
//            //||(event->type() == QEvent::MouseButtonRelease))
//        {
//            m_Time->restart();
//            if(m_bCloseBL)
//            {
//                m_bCloseBL = false;
//                BackLightMgr::open();
//                return true;
//            }
//        }
//        else
//        {
//			if(m_Time->elapsed() >= paramManager->getPanelAddr()->Back_Light_Time*60*1000)
//            {
//                m_bCloseBL = true;
//                BackLightMgr::close();
//            }
//        }
//    }
    return false;
}

/**
  @brief 重新打开背光开关
*/
void StatManager::OpenBackLight()
{
    if(m_bCloseBL)
    {
        m_Time->restart();
        m_bCloseBL = false;
        BackLightMgr::open();
    }
}

/**
  @brief 供X86下测试使用
  */
void StatManager::timerEvent(QTimerEvent *)
{    
    updateFromNormal();
}

/**
* @brief 面板显示风格
*/
void StatManager::panelStyle()
{
    QApplication::setStyle("plastique");
    qApp->setStyleSheet("QPushButton:checked{background-color:rgb(166, 189, 220);"
                                                            "color:white;}");
}


