/**
* @file DigitalKey.cpp
* @brief 公用数字键盘实现
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
#include "digitalKey.h"

/**
* @brief 构造函数
* @param[in] 父窗口指针
*/
DigitalKey::DigitalKey(QWidget *parent)
	: QDialog(parent)
{
	addSubBtnId = InputCount;
	holdTimerId = -1;
	clickTimerId = -1;

	QGridLayout *inputLayout = new QGridLayout(this);
	mapper= new QSignalMapper(this);
	QString css = QString("background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
						  "stop: 0 rgb(165, 179, 198), "
						  "stop: 0.5 rgb(128, 148, 174)"
						  "stop: 1 rgb(114, 136, 175));"
						  "border:1px solid gray;"
						  "border-radius:4px;"
						  "color:white;");
	for (int i = 0; i < InputCount; ++i)
	{
		inputBtn[i] = new PushButton(QString("%1").arg(i + 1));

		if (i == Zero)
			inputBtn[i] ->setText("0");
		else if (i == SubOper)
		{
			inputBtn[i] ->setText(QString(0xfeff2193));
			//inputBtn[i]->setAutoRepeat(true);
			//inputBtn[i]->setAutoRepeatInterval(80);
		}
		else if (i == AddOper)
		{
			inputBtn[i] ->setText(QString(0xfeff2191));
			//inputBtn[i]->setAutoRepeat(true);
			//inputBtn[i]->setAutoRepeatInterval(80);
		}
		else if (i == Clear)
		{
			inputBtn[i] ->setText("clr");
                        inputBtn[i] ->setStyleSheet(css);
		}
		else if (i == Sign)
		{
			inputBtn[i] ->setText("+/-");
                        inputBtn[i] ->setStyleSheet(css);
		}
		else if (i == Backspace)
		{
			inputBtn[i]->setText(QString(0xfeff2190));
                        inputBtn[i] ->setStyleSheet(css);
		}

		inputBtn[i]->setFixedSize(50, 50);
		inputBtn[i]->setFocusPolicy(Qt::NoFocus);

		connect(inputBtn[i], SIGNAL(released()), this, SLOT(inputKeyRelease()));
		connect(inputBtn[i], SIGNAL(pressed()), mapper, SLOT(map()));

		if (i != Zero)
			mapper->setMapping(inputBtn[i], i + 1);
		else
			mapper->setMapping(inputBtn[i], 0);

		inputLayout->addWidget(inputBtn[i], i / 3, i % 3);
		inputLayout->setSpacing(10);
		inputLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
	}
	inputLayout->setMargin(0);

	connect(mapper, SIGNAL(mapped(int)), this, SLOT(inputKeyPress(int)));

	inputBtn[Sign]->hide();
	inputBtn[Backspace]->hide();

}

/**
* @brief 隐藏加减键
*/
void DigitalKey::hideAddSubBtn()
{
	inputBtn[AddOper]->setVisible(false);
	inputBtn[SubOper]->setVisible(false);
}
/**
* @brief 禁止加减键
*/
void DigitalKey::disableAddSubBtn()
{
	inputBtn[AddOper]->setEnabled(false);
	inputBtn[SubOper]->setEnabled(false);
}

/**
* @brief 显示符号键
*/
void DigitalKey::showSignBtn()
{
	inputBtn[Sign]->setVisible(true);
}

/**
* @brief 显示后退键
*/
void DigitalKey::showBackspaceBtn()
{
	inputBtn[Backspace]->setVisible(true);
}

/**
* @brief 禁止数字键盘使用
*/
void DigitalKey::disableNumKey()
{
	for (int i = 0; i < AddOper; ++i)
		inputBtn[i]->setDisabled(true);

	inputBtn[Zero]->setDisabled(true);
}

/**
* @brief 重新实现定时器事件,用于实现加减键连续触发
* @param[in] 定时器事件指针
*/
void DigitalKey::timerEvent(QTimerEvent *ev)
{
	bool doStep = false;
	if (ev->timerId() == holdTimerId)
	{
		killTimer(holdTimerId);
		clickTimerId = startTimer(ClickTimerInterval);
		doStep = true;
	}
	else if (ev->timerId() == clickTimerId)
		doStep = true;

	if (doStep)
	{
		if (addSubBtnId == AddOper)
			emit sendAddOpera();
		else if (addSubBtnId == SubOper)
			emit sendSubOpera();
	}
	QWidget::timerEvent(ev);
}
/**
* @brief 数字键盘抬起槽函数,用于复位定时器
*/
void DigitalKey::inputKeyRelease()
{
	if (addSubBtnId == InputCount)
		return;

	if (holdTimerId != -1)
		killTimer(holdTimerId);

	if (clickTimerId != -1)
		killTimer(clickTimerId);

	holdTimerId = clickTimerId = -1;
	addSubBtnId = InputCount;
}

/**
* @brief 数字键盘按下槽函数
* @param[in] 按键id
*/
void DigitalKey::inputKeyPress(int num)
{
	if (num < AddOper + 1)
		emit sendNum(num);
	else if (num == AddOper + 1)
	{
		holdTimerId = startTimer(HoldTimerInterval);
		addSubBtnId = AddOper;
		emit sendAddOpera();
	}
	else if (num == SubOper + 1)
	{
		holdTimerId = startTimer(HoldTimerInterval);
		addSubBtnId = SubOper;
		emit sendSubOpera();
	}
	else if (num == Clear + 1)
		emit sendClear();
	else if (num == Sign + 1)
		emit sendChangeSign();
	else if (num == Backspace + 1)
		emit sendBackspace();
}

/**
* @brief 停止按键定时发送消息，恢复按键状态
*/
void DigitalKey::stopKeyTimer()
{
	if (holdTimerId != -1)
		killTimer(holdTimerId);

	if (clickTimerId != -1)
		killTimer(clickTimerId);

	holdTimerId = clickTimerId = -1;
	addSubBtnId = InputCount;
}
