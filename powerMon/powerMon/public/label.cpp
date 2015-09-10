/**
* @file label.cpp
* @brief 自定义的label声明
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
#include "label.h"

/**
* @brief 设置显示文本
* @param[in] 文本
* @param[in] 对齐标志
*/
void Label::setCustomText(const QString &text, int flags)
{
        customText = text;
	this->flags = flags;
}

/**
* @brief 绘制label上显示的文字
* @param [in] 描绘事件
*/
void Label::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);

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
