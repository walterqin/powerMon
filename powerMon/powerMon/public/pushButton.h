/**
* @file pushButton.h
* @brief 自定义push型按键定义
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
#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class QLabel;

/**
* @class PushButton
* @brief 自定义push型按键类定义
* @ingroup public
*/
class PushButton : public QPushButton
{
	Q_OBJECT

public:
	/** 显示文本自动换行类型按键 */
	enum Type { WordWrap };

	/** 构造函数 */
	PushButton(QWidget *parent = 0)
		: QPushButton(parent){}

	/** 构造函数 */
	PushButton(const QString &text, QWidget *parent = 0)
		: QPushButton(text, parent){}

	/** 构造函数 */
	PushButton(const QIcon &icon, const QString &text, QWidget *parent = 0)
		: QPushButton(icon, text, parent){}

	PushButton(const Type &type,  QWidget *parent = 0);

	void setWordWrapText(const QString &text);
	void setTextonIcon(const QString &text, int flags);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *ev);

private:
	QLabel *textLabel;            /**< 按键显示文本 */
	QString customText;          /**< 图标上显示文本 */
	int flags;                            /**< 文本位置标志 */

};

#endif // PUSHBUTTON_H
