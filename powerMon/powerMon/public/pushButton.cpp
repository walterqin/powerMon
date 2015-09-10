/**
* @file pushButton.cpp
* @brief 自定义push型按键实现
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
#include "pushButton.h"
#include "statManager.h"

extern StatManager *statMgr;

/**
* @brief 构造函数(支持文本自动换行显示)
* @param[in] 按键类型标志
* @param[in] 父窗口指针
*/
PushButton::PushButton(const Type &type, QWidget *parent)
	: QPushButton(parent)
{
	if (type == WordWrap)
	{
		textLabel = new QLabel;
		textLabel->setWordWrap(true);

		QHBoxLayout *hbox = new QHBoxLayout(this);
		hbox->addWidget(textLabel);
		hbox->setContentsMargins(5, 0, 5, 0);
	}
}

/**
* @brief 重新实现按下事件
* @param[in] 事件指针
*/
void PushButton::mousePressEvent(QMouseEvent *e)
{
	statMgr->startBuzzer();

	return QPushButton::mousePressEvent(e);
}
/**
* @brief 重新实现抬起事件
* @param[in] 事件指针
*/
void PushButton::mouseReleaseEvent(QMouseEvent *e)
{
	return QPushButton::mouseReleaseEvent(e);
}
/**
* @brief 自动换行文本显示接口
* @param[in] 显示文本
*/
void PushButton::setWordWrapText(const QString &text)
{
	textLabel->setText(text);
}

/**
* @brief 在图标上显示文本
* @param[in] 文本内容
* @param[in] 文本显示标志
*/
void PushButton::setTextonIcon(const QString &text, int flags)
{
	customText = text;
	this->flags = flags;
}

/**
* @brief 描绘事件(重新实现)
* @brief 事件指针
*/
void PushButton::paintEvent(QPaintEvent *ev)
{
	QPushButton::paintEvent(ev);

	if (!customText.isEmpty())
	{
		QPainter p(this);
		QFontMetrics fm = p.fontMetrics();

		QPoint offset;
		if (flags == Qt::AlignRight)
		{
			offset.setX(19);
			offset.setY(0);
		}
		QPoint pos((size().width() - fm.width(customText)) / 2 + offset.x(),
				  (size().height() - (fm.ascent() + fm.descent())) / 2 + fm.ascent() + offset.y());

		p.drawText(pos, customText);
	}
}
