#include <QtGui>
#include "paraminputdialog.h"

ParamInputDialog::ParamInputDialog(QString value, QWidget *parent)
    : ParentWindow(parent)
    , oldValue(value)

{
    inputText.clear();
    setupMyUi();
//    forbidLetterinBoard();
}

/**
* @brief 布局显示
*/
void ParamInputDialog::setupMyUi()
{
    defaultTitle();
    topLayoutDisp();
    centerLayoutDisp();
    bottomLayoutDisp();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(myTitleBar);
    layout->setMargin(0);
    layout->addLayout(top);
    layout->addStretch();
    layout->addLayout(center);
    layout->addLayout(bottom);

    setLayout(layout);
    setBtnName();
}

/**
* @brief 输入按键显示区布局
*/
void ParamInputDialog::centerLayoutDisp()
{
    inputPw = new InputKeyboard;
    center = new QVBoxLayout;

    center->addWidget(inputPw);
    center->setContentsMargins(20, 0, 20, 10);

}

/**
* @brief 显示区布局
*/
void ParamInputDialog::topLayoutDisp()
{
    pwLine = new QLineEdit;
    pwLine->setFocusPolicy(Qt::StrongFocus);
    pwLine->setFixedWidth(400);
    pwLine->setFixedHeight(30);
    pwLine->setAlignment(Qt::AlignCenter);
    pwLine->setMaxLength(255);
    pwLine->setText(oldValue);

    top = new QVBoxLayout;

    confirmLine = new QLineEdit;
    confirmLine->setFixedWidth(400);
    confirmLine->setFixedHeight(30);
    confirmLine->setAlignment(Qt::AlignCenter);
    confirmLine->setMaxLength(255);

    inputFlag = new QLabel;
    inputFlag->setText(tr("参数值:"));

    confirmFlag = new QLabel;
    confirmFlag->setText(tr("修改值:"));

    QHBoxLayout *hbox1 = new QHBoxLayout;
    hbox1->addStretch();
    hbox1->addWidget(inputFlag, 0, Qt::AlignCenter);
    hbox1->addWidget(pwLine, 0, Qt::AlignCenter);
    hbox1->addStretch();
    hbox1->setSpacing(10);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addStretch();
    hbox2->addWidget(confirmFlag, 0, Qt::AlignCenter);
    hbox2->addWidget(confirmLine, 0, Qt::AlignCenter);
    hbox2->addStretch();
    hbox2->setSpacing(10);

    top->addLayout(hbox1);
    top->addLayout(hbox2);
    top->setContentsMargins(150, 10, 150, 0);
}

/**
* @brief 底部操作按键布局
*/
void ParamInputDialog::bottomLayoutDisp()
{
    exitBtn = createPushButton(QSize(60, 60),
                               QPixmap("../icons/general/exit.png"), QString(),
                               SLOT(updateButtonTip()), SLOT(close()));
    clrBtn = createPushButton(QSize(60, 60),
                              QPixmap("../icons/general/clear.png"), QString(),
                              SLOT(updateButtonTip()), SLOT(clrInput()));
    backspaceBtn = createPushButton(QSize(60, 60),
                                    QPixmap("../icons/general/clearChar.png"), QString(),
                                    SLOT(updateButtonTip()), SLOT(clrChar()));
    enterBtn = createPushButton(QSize(60, 60),
                                QPixmap("../icons/general/enter.png"), QString(),
                                SLOT(updateButtonTip()), SLOT(enterInput()));

    bottom = new QHBoxLayout;
    bottom->addWidget(exitBtn, 0, Qt::AlignLeft);
    bottom->addStretch();
    bottom->addWidget(clrBtn, 0, Qt::AlignHCenter);
    bottom->addSpacing(20);
    bottom->addWidget(backspaceBtn, 0, Qt::AlignHCenter);
    bottom->addStretch();
    bottom->addWidget(enterBtn, 0, Qt::AlignRight);
    bottom->setContentsMargins(15, 0, 15, 10);
}

/**
* @brief 窗口标题
*/
void ParamInputDialog::defaultTitle()
{
    myTitleBar->setMyWindowTitle(QString("参数配置"));
}

/**
* @brief 确定键槽函数
*/
void ParamInputDialog::enterInput()
{
    defaultTitle();
    inputText = confirmLine->text();
    accept();
}

/**
* @brief 清除键槽函数
*/
void ParamInputDialog::clrInput()
{
    defaultTitle();
    confirmLine->clear();
}

/**
* @brief 设置按键对象名称
*/
void ParamInputDialog::setBtnName()
{
    exitBtn->setObjectName(tr("退出"));
    clrBtn->setObjectName(tr("清除输入"));
    enterBtn->setObjectName(tr("确定"));
    backspaceBtn->setObjectName(tr("清除单个字符"));
}

/**
* @brief 输入板号时禁止输入字母
*/
void ParamInputDialog::forbidLetterinBoard()
{
    inputPw->disableLetter();
}

/**
* @brief 清除光标后字符槽函数
*/
void ParamInputDialog::clrChar()
{
    defaultTitle();
    confirmLine->backspace();
}
