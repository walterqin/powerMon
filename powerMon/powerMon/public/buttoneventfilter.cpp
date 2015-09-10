/**
* @file buttoneventfilter.cpp
* @brief 全局事件过滤器实现
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
#include "buttoneventfilter.h"
#include <qevent.h>
#include <qdebug.h>
#include "statManager.h"

extern StatManager *statMgr;
/**
* @brief 事件过滤器
* @param[in] 目标对象
* @param[in] 事件类型
*/
bool AppEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (statMgr != NULL)
    {
        if (statMgr->closeAutoBackLight(event))
            return true;
    }

    return QObject::eventFilter(obj, event);
}

