/**
* @file environmentmonitordlg.h
* @brief 温湿度监控窗口类定义
* @author qgj
* @date 2015-06-15
* @ingroup gui
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-15    QinGenjian    create
*
*/
#ifndef ENVIRONMENTMONITORDLG_H
#define ENVIRONMENTMONITORDLG_H

#include "../public/parentWindow.h"

class EnvironmentMonitorDlg : public ParentWindow
{
Q_OBJECT
public:
    explicit EnvironmentMonitorDlg(QWidget *parent = 0);

signals:

public slots:

};

#endif // ENVIRONMENTMONITORDLG_H
