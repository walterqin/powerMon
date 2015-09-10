/**
* @file mainWindow.cpp
* @brief 虚主窗口类实现
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
#include "mainwindow.h"

bool MainWindow::powerOnForInit = false;
static QMap<MainWindow::WindowID, MainWindow *> winMap;/**< 主界面对象指针容器 */

/**
* @brief 构造函数
* @param [In] 父指针
*/
MainWindow::MainWindow(QWidget *parent)
    : ParentWindow(parent)
{
    this->setAutoFillBackground(true);
}

/**
* @brief 保存当前界面对象指针
* @param[in] 常用主界面id
* @param[in] 界面指针
*/
void MainWindow::setObjectPointer(WindowID id, MainWindow *p)
{
        winMap.insert(id, p);
}

/**
* @brief 返回指定id界面对象指针
* @param[in] 常用主界面id
*/
MainWindow *MainWindow::getObjectPointer(WindowID id)
{
        if (winMap.contains(id))
                return winMap.value(id);
        else
                return 0;
}

/**
* @brief 初始化虚函数
*/
void MainWindow::init()
{

}

/**
* @brief 右侧布局
*/
void MainWindow::rightZoneLayout()
{
    assayBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/assay.png"),
                                QString(), SLOT(updateButtonTip()), SLOT(onAssay()));
    calibrateBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/cali.png"),
                                    QString(), SLOT(updateButtonTip()), SLOT(onCalibrate()));
    qcBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/qc.png"),
                             QString(), SLOT(updateButtonTip()), SLOT(onQc()));
    maintainBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/maintain.png"),
                                   QString(), SLOT(updateButtonTip()), SLOT(onMaintain()));
    setupBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/setup.png"),
                                QString(), SLOT(updateButtonTip()), SLOT(onSetup()));
    queryBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/query.png"),
                                QString(), SLOT(updateButtonTip()), SLOT(onQuery()));
    infoBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/info.png"),
                               QString(), SLOT(updateButtonTip()), SLOT(onInformation()));
    curcleBtn = createPushButton(QSize(80, 65), QPixmap("../icons/mainwindow/curcle.png"),
                                 QString(), SLOT(updateButtonTip()), SLOT(onCurcle()));

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(assayBtn, 0, 0);
    grid->addWidget(qcBtn, 0, 1);
    grid->addWidget(calibrateBtn, 1, 0);
    grid->addWidget(curcleBtn, 1, 1);
    grid->addWidget(queryBtn, 2, 0);
    grid->addWidget(infoBtn, 2, 1);
    grid->addWidget(maintainBtn, 3, 0);
    grid->addWidget(setupBtn, 3, 1);
    grid->setVerticalSpacing(10);
    grid->setHorizontalSpacing(10);

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(grid);
    rightLayout->addStretch();
    rightLayout->addSpacing(10);
    rightLayout->setContentsMargins(30, 10, 10, 10);

    mainBtnStyleRender();
}

/**
* @brief 样本检测键槽函数
*/
void MainWindow::onAssay()
{
    updateMainTip();

    //TODO: 进行状态的判断，决定是否进行界面切换

    AssayDialog dialog;
    dialog.exec();
}

/**
* @brief 定标键槽函数
*/
void MainWindow::onCalibrate()
{

}

/**
* @brief 质控键槽函数
*/
void MainWindow::onQc()
{

}

/**
* @brief 查询键槽函数
*/
void MainWindow::onQuery()
{

}

/**
* @brief 保养键槽函数
*/
void MainWindow::onMaintain()
{

}

/**
* @brief 设置键槽函数
*/
void MainWindow::onSetup()
{

}

/**
* @brief 信息录入键槽函数
*/
void MainWindow::onInformation()
{

}

/**
* @brief 曲线键槽函数
*/
void MainWindow::onCurve()
{

}

/**
* @brief 获取按键图标
* @param [In] 序号
* @param [In] 图标大小选择
* @return 图标
*/
QPixmap MainWindow::getShapeIcon(int i, bool small = true)
{

}

/**
* @brief 事件过滤器
* @param [In] 对象
* @param [In] 事件
* @return
*/
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{

}

/**
* @brief 上电初始化
*/
void MainWindow::powerOn()
{
    if (!powerOnForInit)
    {
        initSysClock();
        generalParamInfo();
        reportWarnInit();

        //TODO： 上电联机初始化

        powerOnForInit = true;
    }
}

/**
* @brief 初始化系统时钟
*/
void MainWindow::initSysClock()
{

}

/**
* @brief 初始化阶段警告处理
*/
void MainWindow::reportWarnInit()
{

}

/**
* @brief 花样错误恢复处理
* @param[in] 上电初始化标志
*/
void MainWindow::errorToRecovery(bool init = false)
{

}

/**
* @brief 设置按键名称
*/
void MainWindow::setFuncBtnName()
{
    assayBtn->setObjectName(tr("样本测试"));;
    calibrateBtn->setObjectName(tr("曲线定标"));;
    qcBtn->setObjectName(tr("质量控制"));;
    maintainBtn->setObjectName(tr("维修保养"));;
    setupBtn->setObjectName(tr("系统调试"));;
    queryBtn->setObjectName(tr("记录查询"));;
    infoBtn->setObjectName(tr("信息录入"));;
    curcleBtn->setObjectName(tr("反应曲线"));;
}

/**
* @brief 按键风格渲染
*/
void MainWindow::mainBtnStyleRender()
{

}
