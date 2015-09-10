/**
* @file toolButton.h
* @brief 自定义tool型按键定义
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
#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>

/**
* @class ToolButton
* @brief 自定义tool型按键类定义
* @ingroup public
*/
class ToolButton : public QToolButton
{
	Q_OBJECT

public:
	ToolButton(QWidget *parent = 0)
		: QToolButton(parent) {}

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

};

#endif // TOOLBUTTON_H
