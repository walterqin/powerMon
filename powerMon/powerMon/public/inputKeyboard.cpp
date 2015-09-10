/**
* @file password.h
* @brief 公用输入字符键盘类实现
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
#include "inputKeyboard.h"

/**
* @brief 构造函数
* @param[in] 父窗口指针
*/
InputKeyboard::InputKeyboard(QWidget *parent)
    : QDialog(parent)
{
    QString text = "1 2 3 4 5 6 7 8 9 0 "
                   "A B C D E F G H I J "
                   "K L M N O P Q R S T "
                   "U V W X Y Z . , @ ";
    QStringList list = text.split(" ");
    mapper = new QSignalMapper(this);
    center = new QGridLayout;

    for (int i = 0; i < BtnCount; ++i)
    {
            inputBtn[i] = new PushButton(list.at(i));
            inputBtn[i]->setFocusPolicy(Qt::NoFocus);
            inputBtn[i]->setFixedSize(55, 55);

            connect(inputBtn[i], SIGNAL(pressed()), mapper, SLOT(map()));
            mapper->setMapping(inputBtn[i], list.at(i));

            center->addWidget(inputBtn[i], i / 10, i % 10);
            center->setVerticalSpacing(8);
            center->setHorizontalSpacing(20);

    }
    center->setMargin(0);

    setLayout(center);

    connect(mapper, SIGNAL(mapped(QString)), this, SIGNAL(sendText(QString)));
    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(sendEvent(QString)));
}

/**
* @brief 禁止字母键输入
*/
void InputKeyboard::disableLetter()
{
    for (int i = CharA; i < BtnCount; ++i)
        inputBtn[i]->setDisabled(true);
}

/**
* @brief 发送虚拟按键事件
* @param[in] 按键显示文本
*/
void InputKeyboard::sendEvent(const QString &str)
{
    QString text = str;
    if (!isUpper)
        text = str.toLower();

    QKeyEvent keyEvent(QEvent::KeyPress, 0, Qt::NoModifier, text, false, 1);
    QApplication::sendEvent(qApp->focusWidget(), &keyEvent);
}
/**
* @brief 设置大小写
* @param[in] 大写标志
*/
void InputKeyboard::setUpper(bool upper)
{
    isUpper = upper;
    for (int i = 0; i < BtnCount; ++i)
    {
        if (isUpper)
            inputBtn[i]->setText(inputBtn[i]->text().toUpper());
        else
            inputBtn[i]->setText(inputBtn[i]->text().toLower());
    }
}
/**
* @brief 获取大小写标志
* @return 大小写标志
* @retval true 大写
* @retval false 小写
*/
bool InputKeyboard::getUpper() const
{
    return isUpper;
}
