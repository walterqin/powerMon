/**
* @file  machineType.h
* @brief 机型文件
* @author qgj
* @date 2015-06-04
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/
#ifndef MACHINECONFIG_H
#define MACHINECONFIG_H

#include "configure/configure.h"
#include <QString>
#include <QStringList>
#include <QMap>

/**
* @brief 机型
*/
#define RPM1000           0x02  /**<  */

/** @brief 软件版本显示机型定义 */
#define MachineType     "RPM1000"


/** @brief 厂家定义 */
enum {
        RuiWei = 10,             /**< 瑞威 */
};

/** @brief 厂家定义 */
#define FACTORY RuiWei

/** @brief 机型定义 */
#define Machine_Type    RPM1000

/** @brief 最大测试项目数 */
#define MAXTESTITEM     6

/**
* @brief 机型配置类
*/
class MachineConfig
{
public:
    /** 构造函数 */
    MachineConfig() {};

    static int getMachine();
    static int getFactory();
    static QString getMachineName();
    static QString getFactoryID();
    static QString getFactoryCode() ;
    static QString getDatabasePath();
    static QStringList getLayoutNum();
    static QList<int> getContainerType();
    static QStringList getSampleLayout();
    static QStringList getReagentLayout();
    static QStringList getIncubatorLayout();
    static QStringList getTestLayout();
    static QStringList getOtherLayout();
    static QStringList getTestItem();
    static int getMaxItemNum();
    static QMap<double, double> getProprety(int type = 0);

private:
    static int machineType;    /**< 机型代码 */
    static int factoryCode;    /**< 厂家代号 */

};

#endif // MACHINECONFIG_H
