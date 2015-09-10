/**
* @file label.h
* @brief 自定义label定义
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
#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
/**
* @brief 自定义label类定义
* @ingroup public
*/
class Label : public QLabel
{
	Q_OBJECT

public:
	/** 构造函数 */
	Label (QWidget *parent = 0, Qt::WindowFlags f = 0)
		: QLabel(parent, f) {};

	/** 构造函数 */
	Label (const QString &text, QWidget *parent = 0, Qt::WindowFlags f = 0)
		: QLabel(text, parent, f) {};

	void setCustomText(const QString &text, int flags);

protected:
	void paintEvent(QPaintEvent *ev);

private:
	QString customText;    /**< 显示文本 */
        int flags;             /**< 文本位置标志 */

};

#endif // LABEL_H
