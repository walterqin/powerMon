/**
* @file buttoneventfilter.h
* @brief 全局事件过滤器定义
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
#ifndef BUTTONEVENTFILTER_H
#define BUTTONEVENTFILTER_H

#include <QObject>
/**
* @class AppEventFilter
* @brief 全局事件过滤器类定义
* @ingroup public
*/
class AppEventFilter : public QObject
{
	Q_OBJECT
public:
	/** @brief 构造函数 */
    AppEventFilter(){};

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif // BUTTONEVENTFILTER_H
