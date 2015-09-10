/**
* @file parentWindow.cpp
* @brief 自定义父窗口实现
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
#include "parentWindow.h"
#include "public/buttoneventfilter.h"


bool ParentWindow::powerOnForInit = false;

/**
* @brief 构造函数
* @param[in] 父窗口指针
*/
ParentWindow::ParentWindow(QWidget *parent)
	: QDialog(parent, Qt::FramelessWindowHint)
{
#ifdef X86_COMPILE
    setStyleSheet("font-family: wqy-micro; font-size: 18px;");
#endif
    setFixedSize(X_SIZE, Y_SIZE);

    myTitleBar = new TitleBar;
    move(0, 0);
}
/**
* @brief 创建PushButton对象接口函数
* @param[in] 按键大小
* @param[in] 按键图标
* @param[in] 按键文本
* @param[in] 按下触发的槽函数
* @param[in] 抬起触发的槽函数
*/
PushButton *ParentWindow::createPushButton(const QSize &size, const QPixmap &pix,
                        const QString &text, const char *member1, const char *member2)
{
    PushButton *button = new PushButton;

    button->setFixedSize(size);
    button->setFocusPolicy(Qt::NoFocus);
    if (!pix.isNull())
    {
        button->setIcon(pix);
        button->setIconSize(pix.size());
    }
    if (!text.isEmpty())
    {
        button->setText(text);
    }

    connect(button, SIGNAL(pressed()), this, member1);
    connect(button, SIGNAL(released()), this, member2);

    return button;
}
/**
* @brief 创建PushButton对象接口函数(重载)
* @param[in] 按键大小
* @param[in] 按键图标
* @param[in] 按键文本
* @param[in] 触发的槽函数
*/
PushButton *ParentWindow::createPushButton(const QSize &size, const QPixmap &pix,
                        const QString &text, const char *member)
{
    PushButton *button = new PushButton;

    button->setFixedSize(size);
    button->setFocusPolicy(Qt::NoFocus);
    if (!pix.isNull())
    {
        button->setIcon(pix);
        button->setIconSize(pix.size());
    }
    if (!text.isEmpty())
    {
        button->setText(text);
    }

    connect(button, SIGNAL(clicked()), this, member);

    return button;
}
/**
* @brief 创建ToolButton对象接口函数
* @param[in] 按键大小
* @param[in] 按键图标
* @param[in] 按键文本
* @param[in] 工具键箭头类型
* @param[in] 按下触发的槽函数
* @param[in] 抬起触发的槽函数
*/
ToolButton *ParentWindow::createToolButton(const QSize &size, const QPixmap &pix,
                                            const QString &text, Qt::ArrowType type,
                                            const char *member1, const char *member2)
{
    ToolButton *button = new ToolButton;

    button->setFixedSize(size);
    button->setFocusPolicy(Qt::NoFocus);
    button->setArrowType(type);
    if (!pix.isNull())
    {
        button->setIcon(pix);
        button->setIconSize(size);
    }
    if (!text.isEmpty())
    {
        button->setText(text);
    }

    connect(button, SIGNAL(pressed()), this, member1);
    connect(button, SIGNAL(released()), this, member2);

    return button;
}

/**
* @brief 创建ToolButton对象接口函数
* @param[in] 按键大小
* @param[in] 按键图标
* @param[in] 按键文本
* @param[in] 工具键箭头类型
* @param[in] 按下触发的槽函数
* @param[in] 抬起触发的槽函数
*/
ToolButton *ParentWindow::createToolButton(const QSize &size, const QPixmap &pix,
                                            const QString &text, Qt::ArrowType type,
                                            const char *member)
{
    ToolButton *button = new ToolButton;

    button->setFixedSize(size);
    button->setFocusPolicy(Qt::NoFocus);
    button->setArrowType(type);
    if (!pix.isNull())
    {
        button->setIcon(pix);
        button->setIconSize(size);
    }
    if (!text.isEmpty())
    {
        button->setText(text);
    }

    connect(button, SIGNAL(clicked()), this, member);

    return button;
}

/**
* @brief 按键按下槽函数，用于刷新标题栏信息
* @note大多数按键按下后刷新标题栏信息（支持连续按的按键除外）
*/
void ParentWindow::updateButtonTip()
{
    QAbstractButton *button = qobject_cast<QAbstractButton *>(QObject::sender());
    myTitleBar->setMyWindowTitle(button->objectName());
}
/**
* @brief 按键按下槽函数,不刷新标题栏信息
* @see updateButtonTip()
*/
void ParentWindow::noNeedUpdateTip()
{
    return;
}

/**
* @brief 用于刷新标题栏当前窗口信息
* @see updateButtonTip()
*/
void ParentWindow::updateMainTip()
{
    myTitleBar->setMyWindowTitle(this->objectName());
}

/**
* @brief 更新时钟显示
*/
void ParentWindow::updateClock()
{
    myTitleBar->updateTimeDisp();
}
