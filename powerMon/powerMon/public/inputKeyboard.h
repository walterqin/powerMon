/**
* @file inputKeyboard.h
* @brief 公用输入字符键盘定义
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

#ifndef INPUTKEYBOARD_H
#define INPUTKEYBOARD_H

#include <QDialog>
#include "pushButton.h"

class QSignalMapper;
class QGridLayout;

class InputKeyboard : public QDialog
{
    Q_OBJECT

public:
    InputKeyboard(QWidget *parent = 0);

    void disableLetter();
    void setUpper(bool upper);
    bool getUpper() const;

    enum keyboardType {
        AllKeyboard = 0,
        Digitial,
        Letter,
    };

signals:
    /** 发送按键内容信号 */
    void sendText(const QString &str);

private slots:
    void sendEvent(const QString &str);

private:
    /** @brief 字符A和字符总数定义 */
    enum { CharA = 10, BtnCount = 39 };
    PushButton *inputBtn[BtnCount];       /**< 输入按键 */

    QSignalMapper *mapper;               /**< 输入按键翻译器 */
    QGridLayout *center;                 /**< 输入区布局 */

    bool isUpper;                        /**< 大写标志 */
};

#endif // INPUTKEYBOARD_H
