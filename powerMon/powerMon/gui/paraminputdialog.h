#ifndef PARAMINPUTDIALOG_H
#define PARAMINPUTDIALOG_H

#include "../public/parentWindow.h"
#include "../public/inputKeyboard.h"

class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;

class ParamInputDialog : public ParentWindow
{
Q_OBJECT
public:
    explicit ParamInputDialog(QString value, QWidget *parent = 0);
    QString getInputText() const { return inputText;}

signals:

public slots:
    void enterInput();
    void clrInput();
    void clrChar();

private:
    void setupMyUi();
    void topLayoutDisp();
    void centerLayoutDisp();
    void bottomLayoutDisp();
    void defaultTitle();
    void setBtnName();
    void forbidLetterinBoard();

private:
    PushButton *exitBtn;                    /**< 退出键 */
    PushButton *enterBtn;                 /**< 确定键 */
    PushButton *clrBtn;                      /**< 清除键 */
    PushButton *backspaceBtn;         /**< 清除字符键 */

    QLabel *inputFlag;                        /**< 原始数据 */
    QLabel *confirmFlag;                      /**< 修改数据 */
    QLineEdit *pwLine;                         /**< 原始数据显示 */
    QLineEdit *confirmLine;                  /**< 修改数据显示 */
    QSignalMapper *mapper;               /**< 输入按键翻译器 */

    QVBoxLayout *center;                       /**< 输入区布局 */
    QVBoxLayout *top;                          /**< 显示布局 */
    QHBoxLayout *bottom;                    /**< 底层按键布局 */

    InputKeyboard *inputPw;                 /**< 输入对话框 */
    bool firstInput;

private:
    QString oldValue;
    QString inputText;
};

#endif // PARAMINPUTDIALOG_H
