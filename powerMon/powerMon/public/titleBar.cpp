/**
* @file titleBar.cpp
* @brief 自定义标题栏实现
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
#include "titleBar.h"

/**
* @brief 构造函数
* @param[in] 父窗口指针
*/
TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent), dispTime(true)
{
	setAutoFillBackground(true);
	titleBarStyle();

	iconLabel = new QLabel;
	iconLabel->setPixmap(style()->standardPixmap(QStyle::SP_ComputerIcon));

	titleLabel = new QLabel(tr("Window Title"));
	titleLabel->setIndent(20);

	warnLabel = new QLabel;

	timeLabel = new QLabel;
	timerEvent(0);//时钟显示与界面显示同步
	startTimer(1000 * 60);

	iconLabel->setMinimumHeight(20);
	titleLabel->setMinimumHeight(20);
	timeLabel->setMinimumHeight(20);

	QHBoxLayout *hLayout = new QHBoxLayout(this);
	hLayout->addWidget(iconLabel, 0, Qt::AlignLeft);
	hLayout->addWidget(titleLabel);
	hLayout->addWidget(warnLabel);
	hLayout->addSpacing(30);
	hLayout->addWidget(timeLabel);
	hLayout->insertStretch(2, 100);

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	unlockTimer = new QTimer(this);
	connect(unlockTimer, SIGNAL(timeout()), this, SLOT(receiveUnlock()));

}
/**
* @brief 重新实现定时器事件,用于刷新标题栏时钟显示
*/
void TitleBar::timerEvent(QTimerEvent *)
{
	if (dispTime)
	{
		QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm");
		timeLabel->setText(timeStr);
	}
}
/**
* @brief 设置自定义标题栏标题
* @param[in] 标题文本
*/
void TitleBar::setMyWindowTitle(const QString &title)
{
	titleLabel->setText(title);
}

/**
* @brief 获取自定义标题栏标题文本
* @return 标题
*/
QString TitleBar::myWindowTitle() const
{
	return titleLabel->text();
}
/**
* @brief 禁止标题栏时钟显示接口函数
*/
void TitleBar::disabelTimerDisp()
{
	dispTime = false;
	timeLabel->hide();
}
/**
* @brief 更新时间显示槽函数
*/
void TitleBar::updateTimeDisp()
{
	QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd  hh:mm");
	timeLabel->setText(timeStr);
}
/**
* @brief 重新实现鼠标按下事件,用于启动解锁定时器
* @param[in] 事件指针
*/
void TitleBar::mousePressEvent(QMouseEvent *ev)
{
	QWidget::mousePressEvent(ev);
//	if (paramManager->getUsedParamAddr()->LockPanel)
//		unlockTimer->start(2500);
//    else if(alarmTip)
        emit openAlarmTip();
}
/**
* @brief 重新实现鼠标抬起事件,用于停止解锁定时器
* @param[in] 事件指针
*/
void TitleBar::mouseReleaseEvent(QMouseEvent *ev)
{
	QWidget::mouseReleaseEvent(ev);
	if (unlockTimer->isActive())
		unlockTimer->stop();

    if(alarmTip)
        emit closeAlarmTip();
}
/**
* @brief 解锁定时器槽函数
* @note 主窗口下持续按住窗口标题几秒钟,可以解锁键盘
*/
void TitleBar::receiveUnlock()
{
	emit unlockPanel();
	//qDebug() << "receive unlock";
}

/**
* @brief 标题栏显示风格
*/
void TitleBar::titleBarStyle()
{
	setBackgroundRole(QPalette::Highlight);
	update();
}

/**
* @brief 刷新标题栏提示信息
* @param[in] 报警标志
*/
void TitleBar::updateWarnFlag(bool warn)
{
    alarmTip = warn;

	if (warn)
		warnLabel->setPixmap(style()->standardPixmap(QStyle::SP_MessageBoxWarning));
	else
		warnLabel->setPixmap(QPixmap());
}

