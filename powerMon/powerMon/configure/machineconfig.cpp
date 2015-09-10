/**
* @file  machineConfig.cpp
* @brief 机型配置类实现
* @author qgj
* @date 2015-06-04
* @ingroup configure
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/
#include "machineconfig.h"
#include <QDebug>


int MachineConfig::machineType = Machine_Type;
int MachineConfig::factoryCode = FACTORY;

/**
* @brief 获取机型
* @return 机型
*/
int MachineConfig::getMachine()
{
    return machineType;
}

/**
* @brief 获取厂家
* @return 厂家
*/
int MachineConfig::getFactory()
{
    return factoryCode;
}

/**
* @brief 获取机型显示名称
* @return 机型显示名称
*/
QString MachineConfig::getMachineName()
{
    return QString(MachineType);
}

/**
* @brief 获取厂家显示代码
* @return 厂家显示代码
*/
QString MachineConfig::getFactoryCode()
{
    return QString();
}

/**
* @brief 获取厂家ID
* @return 厂家ID
*/
QString MachineConfig::getFactoryID()
{
    return QString();
}


/**
* @brief 获取数据库路径
* @retval 路径
*/
QString MachineConfig::getDatabasePath()
{
    return QString("/root/db/");
}

/**
* @brief 获取试剂位布局
* @retval 试剂位列表
*/
QStringList MachineConfig::getLayoutNum()
{
    QStringList list;
    list << QString("A1") << QString("A2") << QString("A3") << QString("A4") << QString("A5") << QString("A6") << QString("A7") << QString("A8") << QString("A9") << QString("A10") << QString("A11") << QString("A12")
         << QString("B1") << QString("B2") << QString("B3") << QString("B4") << QString("B5") << QString("B6") << QString("B7") << QString("B8") << QString("B9") << QString("B10") << QString("B11") << QString("B12")
         << QString("C1") << QString("C2") << QString("C3") << QString("C4") << QString("C5") << QString("C6") << QString("C7") << QString("C8") << QString("C9") << QString("C10") << QString("C11") << QString("C12")
         << QString("D1") << QString("D2") << QString("D3") << QString("D4") << QString("D5") << QString("D6") << QString("D7") << QString("D8") << QString("D9") << QString("D10") << QString("D11") << QString("D12")
         << QString("H1") << QString("H2") << QString("H3") << QString("H4") << QString("H5") << QString("H6") << QString("H7") << QString("H8") << QString("H9") << QString("H10") << QString("H11")
         << QString("I1") << QString("I2") << QString("I3") << QString("I4") << QString("I5") << QString("I6") << QString("I7") << QString("I8") << QString("I9") << QString("I10") << QString("I11")
         << QString("J1") << QString("J2") << QString("J3") << QString("J4") << QString("J5") << QString("J6") << QString("J7") << QString("J8") << QString("J9") << QString("J10") << QString("J11")
         << QString("K1") << QString("K2") << QString("K3") << QString("K4") << QString("K5") << QString("K6") << QString("K7") << QString("K8") << QString("K9") << QString("K10") << QString("K11")
         << QString("L1") << QString("L2")
         << QString("M1") << QString("M2")
         << QString("N1") << QString("N2")
         << QString("O1") << QString("02")
         << QString("P1") << QString("P2")
         << QString("Q1")
         << QString("E1") << QString("E2") << QString("E3") << QString("E4") << QString("E5")
         << QString("F1") << QString("F2") << QString("F3") << QString("F4") << QString("F5")
         << QString("G1") << QString("G2") << QString("G3") << QString("G4") << QString("G5") << QString("G6") << QString("G7") << QString("G8") << QString("G9") << QString("G10")
         << QString("R1") << QString("R2")
         << QString("S1")
         << QString("T1");
    return list;
}


QMap<double, double> MachineConfig::getProprety(int type)
{
    QMap<double, double> map;
    switch(type)
    {
    default:
        map[0.0] = 0x0;
        map[5.0] = 2000.0;
        map[10.0] = 4000.0;
        break;
    }
    return map;
}

QList<int> MachineConfig::getContainerType()
{
    QList<int> list;
    list.clear();
    return list;
}

/**
* @brief 获取样本位布局
* @retval 样本列表
*/
QStringList MachineConfig::getSampleLayout()
{
    QStringList list;
    list << QString("A1") << QString("A2") << QString("A3") << QString("A4") << QString("A5") << QString("A6") << QString("A7") << QString("A8") << QString("A9") << QString("A10") << QString("A11") << QString("A12")
         << QString("B1") << QString("B2") << QString("B3") << QString("B4") << QString("B5") << QString("B6") << QString("B7") << QString("B8") << QString("B9") << QString("B10") << QString("B11") << QString("B12")
         << QString("C1") << QString("C2") << QString("C3") << QString("C4") << QString("C5") << QString("C6") << QString("C7") << QString("C8") << QString("C9") << QString("C10") << QString("C11") << QString("C12")
         << QString("D1") << QString("D2") << QString("D3") << QString("D4") << QString("D5") << QString("D6") << QString("D7") << QString("D8") << QString("D9") << QString("D10") << QString("D11") << QString("D12");
    return list;
}

/**
* @brief 获取试剂位布局
* @retval 试剂位列表
*/
QStringList MachineConfig::getReagentLayout()
{
    QStringList list;
    list << QString("H1") << QString("H2") << QString("H3") << QString("H4") << QString("H5") << QString("H6") << QString("H7") << QString("H8") << QString("H9") << QString("H10") << QString("H11")
         << QString("I1") << QString("I2") << QString("I3") << QString("I4") << QString("I5") << QString("I6") << QString("I7") << QString("I8") << QString("I9") << QString("I10") << QString("I11")
         << QString("J1") << QString("J2") << QString("J3") << QString("J4") << QString("J5") << QString("J6") << QString("J7") << QString("J8") << QString("J9") << QString("J10") << QString("J11")
         << QString("K1") << QString("K2") << QString("K3") << QString("K4") << QString("K5") << QString("K6") << QString("K7") << QString("K8") << QString("K9") << QString("K10") << QString("K11")
         << QString("L1") << QString("L2")
         << QString("M1") << QString("M2")
         << QString("N1") << QString("N2")
         << QString("O1") << QString("O2")
         << QString("P1") << QString("P2")
         << QString("Q1");
    return list;
}

/**
* @brief 获取预温位布局
* @retval 预温位列表
*/
QStringList MachineConfig::getIncubatorLayout()
{
    QStringList list;
    list << QString("E1") << QString("E2") << QString("E3") << QString("E4") << QString("E5")
         << QString("F1") << QString("F2") << QString("F3") << QString("F4") << QString("F5");
    return list;
}

/**
* @brief 获取测试位布局
* @retval 测试位列表
*/
QStringList MachineConfig::getTestLayout()
{
    QStringList list;
    list << QString("G1") << QString("G2") << QString("G3") << QString("G4") << QString("G5") << QString("G6") << QString("G7") << QString("G8") << QString("G9") << QString("G10");
    return list;
}

/**
* @brief 获取其他位置布局
* @retval 其他位置列表
*/
QStringList MachineConfig::getOtherLayout()
{
    QStringList list;
    list << QString("R1") << QString("R2")
         << QString("S1")
         << QString("T1");
    return list;
}

/**
* @brief 获取测试项目
* @retval 测试项目
*/
QStringList MachineConfig::getTestItem()
{
    return QStringList();
}

/**
* @brief 获取最大测试项目数
* @retval 最大项目数
*/
int MachineConfig::getMaxItemNum()
{
    return MAXTESTITEM;
}
