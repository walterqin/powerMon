/**
* @file powermonitordlg.cpp
* @brief 电源监控窗口类实现
* @author qgj
* @date 2015-06-09
* @ingroup gui
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-09    QinGenjian    create
*
*/

#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "../database/profile.h"
#include "powermonitordlg.h"
#include "commu/commanager.h"
#include "commu/cominfo.h"

extern ComManager* comManager;
extern ComInfo *mctrInfo;

/**
* @brief 构造函数
* @param [I] 父指针
*/
PowerMonitorDlg::PowerMonitorDlg(QWidget *parent)
    : ParentWindow(parent)
{
    init();
    setupUi();

    for (int i = 0; i < num; ++i)
        renderStatusLamp(i, statusLampFlag[i]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onFlashText()));
    timer->start(300);

    renderTimer = new QTimer(this);
    connect(renderTimer, SIGNAL(timeout()), this, SLOT(onGetPowerRelay()));
    renderTimer->start(2000);

    connect(mctrInfo, SIGNAL(powerParamChanged()), this, SLOT(onRenderPowerParamter()));
    connect(mctrInfo, SIGNAL(relayStatusChanged()), this, SLOT(onRenderRelayStatus()));
}

/**
* @brief 初始化函数
*/
void PowerMonitorDlg::init()
{
    for (int i = 0; i < MaxItem; i++)
    {
        statusLampFlag[i] = false;
        vol[i] = 0;
        cur[i] = 0;
    }

    flash = false;

    db = profile.dataDatabase();

    getConfigInfo();
    getInitStatus();
}

/**
* @brief 创建窗体布局
*/
void PowerMonitorDlg::setupUi()
{
    defaultTitle();
    createWidget();

    QHBoxLayout *keyLayout = new QHBoxLayout;
    keyLayout->addStretch();
    keyLayout->addWidget(exitBtn);
    keyLayout->addSpacing(50);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myTitleBar);
    mainLayout->setMargin(0);
    mainLayout->addLayout(gridLayout);
    mainLayout->addSpacing(5);
    mainLayout->addStretch();
    mainLayout->addLayout(keyLayout);
    mainLayout->setContentsMargins(10, 5, 5, 2);

    setLayout(mainLayout);
}

/**
* @brief 创建窗体控件
*/
void PowerMonitorDlg::createWidget()
{
    int maxWay = (num > MaxItem ? MaxItem : num);

    exitBtn = createPushButton(QSize(60, 55), QPixmap("../icons/general/enter.png"), QString(""),
                               SLOT(updateButtonTip()), SLOT(onExit()));
    exitBtn->setObjectName(QString("返回"));

    gridLayout = new QGridLayout;
    QSignalMapper *mapper1 = new QSignalMapper(this);
    QSignalMapper *mapper2 = new QSignalMapper(this);
    for (int i = 0; i < maxWay; i ++)
    {
        openBtn[i] = createPushButton(QSize(50, 30), QPixmap(), QString("打开"),
                                      SLOT(updateButtonTip()), SLOT(updateButtonTip()));
        closeBtn[i] = createPushButton(QSize(50, 30), QPixmap(), QString("关闭"),
                                       SLOT(updateButtonTip()), SLOT(updateButtonTip()));

        openBtn[i]->setObjectName(QString("打开%1#开关").arg(i + 1, 2, 10, QLatin1Char('0')));
        closeBtn[i]->setObjectName(QString("关闭%1#开关").arg(i + 1, 2, 10, QLatin1Char('0')));

        connect(openBtn[i], SIGNAL(released()), mapper1, SLOT(map()));
        connect(closeBtn[i], SIGNAL(released()), mapper2, SLOT(map()));

        mapper1->setMapping(openBtn[i], i);
        mapper2->setMapping(closeBtn[i], i);

        switchType[i] = new QGroupBox(QString("#%1").arg(i + 1, 2, 10, QLatin1Char('0')));

        voltageLbl[i] = new QLabel;
        voltageLbl[i]->setText(QString("电压:"));

        currentLbl[i] = new QLabel;
        currentLbl[i]->setText(QString("电流:"));

        statusLbl[i] = new QLabel;
        statusLbl[i]->setText(QString("电源状态"));

        voltageEdt[i] = new QLineEdit;
        voltageEdt[i]->setText(QString("%1").arg(vol[i]));
        voltageEdt[i]->setAlignment(Qt::AlignCenter);

        currentEdt[i] = new QLineEdit;
        currentEdt[i]->setText(QString("%1").arg(cur[i]));
        currentEdt[i]->setAlignment(Qt::AlignCenter);

        statusLamp[i] = new QLabel;
        statusLamp[i]->setFixedSize(20, 20);
        statusLamp[i]->setStyleSheet("background-color:black");
        statusLamp[i]->setFrameStyle(QFrame::Box);

        QHBoxLayout *voltLayout = new QHBoxLayout;
        voltLayout->addWidget(voltageLbl[i]);
        voltLayout->addSpacing(5);
        voltLayout->addWidget(voltageEdt[i]);

        QHBoxLayout *currLayout = new QHBoxLayout;
        currLayout->addWidget(currentLbl[i]);
        currLayout->addSpacing(5);
        currLayout->addWidget(currentEdt[i]);

        QHBoxLayout *statLayout =  new QHBoxLayout;
        statLayout->addWidget(statusLbl[i]);
        statLayout->addSpacing(20);
        statLayout->addWidget(statusLamp[i]);
        statLayout->addStretch();

        QHBoxLayout *keyLayout = new QHBoxLayout;
        keyLayout->addWidget(openBtn[i]);
        keyLayout->addSpacing(15);
        keyLayout->addWidget(closeBtn[i]);

        QVBoxLayout *powerLayout = new QVBoxLayout;
        powerLayout->addLayout(voltLayout);
        powerLayout->addLayout(currLayout);
        powerLayout->addLayout(statLayout);
        powerLayout->addLayout(keyLayout);
        powerLayout->addStretch();
        switchType[i]->setLayout(powerLayout);

        gridLayout->addWidget(switchType[i], i / 4, i % 4);
    }
    connect(mapper1, SIGNAL(mapped(int)), this, SLOT(onOpenPower(int)));
    connect(mapper2, SIGNAL(mapped(int)), this, SLOT(onClosePower(int)));

    gridLayout->setVerticalSpacing(3);
    gridLayout->setHorizontalSpacing(30);
}

/**
* @brief 标题栏
*/
void PowerMonitorDlg::defaultTitle()
{
    myTitleBar->setMyWindowTitle(tr("电源输出监控"));
}

/**
* @brief 退出键槽函数
*/
void PowerMonitorDlg::onExit()
{
//    timer->stop();
    renderTimer->stop();
    close();
}

/**
* @brief 打开指定路的电源
* @param [I] 路号
*/
void PowerMonitorDlg::onOpenPower(int num)
{
    QString flagFile = QString("touch /tmp/open%1").arg(num + 1);
    system(flagFile.toAscii().data());
//    renderStatusLamp(num, true);
}

/**
* @brief 关闭指定路的电源
* @param [I] 路号
*/
void PowerMonitorDlg::onClosePower(int num)
{
    QString flagFile = QString("touch /tmp/close%1").arg(num + 1);
    system(flagFile.toAscii().data());
//    renderStatusLamp(num, false);
}

/**
* @brief 状态指示灯刷新
* @param [I] 灯号
* @param [I] 标志
*/
void PowerMonitorDlg::renderStatusLamp(int lamp, bool flag)
{
    int maxWay = (num > MaxItem ? MaxItem : num);

    if (lamp > maxWay)
        return;

    if (flag)
    {
        statusLamp[lamp]->setStyleSheet("background-color:red");
    }
    else
    {
        statusLamp[lamp]->setStyleSheet("background-color:black");
    }
}

/**
* @brief 读取系统初始状态信息
*/
void PowerMonitorDlg::getInitStatus()
{
    int wayNum = (num > MaxItem ? MaxItem : num);

    for (int i = 0; i < wayNum; ++i)
    {
        if (flag.at(i).toInt() == 1)
            statusLampFlag[i] = true;
        else
            statusLampFlag[i] = false;
    }
}

/**
* @brief 读取系统配置信息
*/
void PowerMonitorDlg::getConfigInfo()
{
    QString defValue = profile.value("keyDefault").toString();

    num = profile.value("maxWay").toStringList().at(0).toInt(); //读取配置的路数
    flag = profile.value("keyStatus").toStringList().at(0).split(";");

    if (flag.count() < num)
    {
        for (int i = 0; i < num - flag.count() + 1; i++)
            flag.append(defValue);
    }
}

/**
* @brief 刷新电力参数
*/
void PowerMonitorDlg::onRenderPowerParamter()
{
    for (int i = 0; i < MachineConfig::getMaxItemNum(); i++)
    {
        PowParam pp;
        mctrInfo->getPowerParam(i, pp);
        vol[i] = pp.vol;
        cur[i] = pp.cur;

        voltageEdt[i]->setText(QString("%1").arg(vol[i]));
        currentEdt[i]->setText(QString("%1").arg(cur[i]));
    }
}

/**
* @brief 更新继电器状态
*/
void PowerMonitorDlg::onRenderRelayStatus()
{
//    int relay = mctrInfo->getRelayStatus();
}

/**
* @brief 参数数字刷新
*/
void PowerMonitorDlg::onFlashText()
{
    for (int i = 0; i < MachineConfig::getMaxItemNum(); i++)
    {
        if (flash)
        {
            voltageEdt[i]->setText(QString("%1").arg(vol[i]));
            currentEdt[i]->setText(QString("%1").arg(cur[i]));
        }
        else
        {
            voltageEdt[i]->setText("");
            currentEdt[i]->setText("");
        }

        renderStatusLamp(i, statusLampFlag[i]);
    }

    if (flash)
        flash = false;
    else
        flash = true;
}

/**
* @brief
*/
void PowerMonitorDlg::onGetPowerRelay()
{
    QSqlQuery q(profile.dataDatabase());

    for (int i = 0; i < MachineConfig::getMaxItemNum(); i++)
    {
        QString sql = QString("select volt, curr, switch from power where waynum = '%1'")
                      .arg(i + 1);
        q.exec(sql);

        if (q.next())
        {
            vol[i] = q.value(0).toInt();
            cur[i] = q.value(1).toInt();
            statusLampFlag[i] = q.value(2).toBool();
        }
        sql.clear();
    }
}

/**
* @brief 背景刷新
* @param 刷新事件
*/
void PowerMonitorDlg::paintEvent(QPaintEvent *ev)
{
    QDialog::paintEvent(ev);

    QPainter pp(this);
    pp.setPen(Qt::NoPen);
    pp.setBrush(QColor(182, 147, 139));
    pp.drawRect(QRect(10, 410, 780, 5));
}
