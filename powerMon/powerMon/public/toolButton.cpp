/**
* @file toolButton.cpp
* @brief 自定义tool型按键实现
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

#include <QtGui>
#include "toolButton.h"
#include "statManager.h"

extern StatManager *statMgr;

/**
* @brief 重新实现按下事件
* @param[in] 事件指针
*/
void ToolButton::mousePressEvent(QMouseEvent *e)
{
	statMgr->startBuzzer();
	return QToolButton::mousePressEvent(e);
}
/**
* @brief 重新实现抬起事件
* @param[in] 事件指针
*/
void ToolButton::mouseReleaseEvent(QMouseEvent *e)
{
	return QToolButton::mouseReleaseEvent(e);
}

