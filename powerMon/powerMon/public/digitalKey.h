/**
* @file digitalKey.h
* @brief 公用数字键盘定义
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
#ifndef DIGITALKEY_H
#define DIGITALKEY_H

#include <QDialog>
#include "pushButton.h"

class QSignalMapper;

/** @a repeat型按键时间间隔定义 */
enum {
	HoldTimerInterval = 500, /**< 按键保持时间间隔 */
	ClickTimerInterval = 100  /**< 连续按下时间间隔定义(ms) */
};

/**
* @class DigitalKey
* @brief 输入数字键盘类定义
* @ingroup public
*/
class DigitalKey : public QDialog
{
	Q_OBJECT

public:
    DigitalKey(QWidget *parent = 0);

    void hideAddSubBtn();
    void disableAddSubBtn();
    void showSignBtn();
    void showBackspaceBtn();
    void disableNumKey();
    void stopKeyTimer();

    /** 符号操作键定义 */
    enum {
        AddOper = 9,         /**< 加键  */
        SubOper = 10,       /**< 减键 */
        Zero = 11,             /**< 零 */
        Clear = 12,            /**< 清除键 */
        Sign = 13,             /**< 符号键 */
        Backspace = 14,   /**< 后退键 */
        InputCount = 15     /**< 按键总数 */
    };

signals:
    /** @brief 发送数字键盘信号 */
    void sendNum(int num);

    /** @brief 发送加操作信号 */
    void sendAddOpera();

    /** @brief 发送减操作信号 */
    void sendSubOpera();

    /** @brief 发送清除信号 */
    void sendClear();

    /** @brief 发送回退信号 */
    void sendBackspace();

    /** @brief 发送更改符号信号 */
    void sendChangeSign();

protected:
    void timerEvent(QTimerEvent *ev);

private slots:
    void inputKeyPress(int num);
    void inputKeyRelease();

private:
    QSignalMapper *mapper;                    /**< 数字键盘信号mapper */
    PushButton *inputBtn[ InputCount ];     /**< 数字键盘按键 */

    int holdTimerId;                                    /**< 按键连续按下保持时间定时器id */
    int clickTimerId;                                    /**< 按键连续按下时间间隔定时器id */
    int addSubBtnId;                                   /**< 加减键id */
};

#endif // DIGITALKEY_H
