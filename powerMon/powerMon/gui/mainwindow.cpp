/**
* @file mainWindow.cpp
* @brief 主窗口类实现
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
#include "powermonitordlg.h"
#include "logquerydlg.h"
#include "parameterconfigdlg.h"
#include "dataquerydlg.h"
#include "commu/commanager.h"
#include "commu/cominfo.h"

//
extern ComManager *comManager;
extern ComInfo *mctrInfo;

bool MainWindow::powerOnForInit = false;
static QMap<WindowID, MainWindow *> winMap;/**< 主界面对象指针容器 */

/**
* @brief 构造函数
* @param [In] 父指针
*/
MainWindow::MainWindow(QWidget *parent)
    : ParentWindow(parent)
{
    this->setAutoFillBackground(true);

    nameList.clear();
    iconList.clear();

    init();
    setupMainWindow();
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
    nameList << QString(tr("电源控制"))<< QString(tr("环境状态")) << QString(tr("数据查询")) << QString(tr("日志查询")) << QString(tr("系统设置"));
    iconList << "power.png" << "environment.png" << "querydata.png" << "querylog.png" << "setting.png";
}

/**
* @brief 右侧布局
*/
void MainWindow::setupMainWindow()
{
    btnGroup = new QButtonGroup;

    QGridLayout *grid = new QGridLayout;
    QSignalMapper *mapper = new QSignalMapper(this);

    QPixmap bmp;
    QString name;
    for (int i = 0; i < FunCount; i++)
    {
        bmp = getFuncIcon(i);
        funcBtn[i] = createPushButton(QSize(97, 132), bmp, QString(),
                                      SLOT(updateButtonTip()), SLOT(updateButtonTip()));
        name = getFuncName(i);
        funcLbl[i] = new Label;
        funcLbl[i]->setCustomText(name, Qt::AlignCenter);

        funcBtn[i]->setObjectName(name);
        btnGroup->addButton(funcBtn[i]);

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(funcBtn[i]);
        vbox->addWidget(funcLbl[i]);

        connect(funcBtn[i], SIGNAL(released()), mapper, SLOT(map()));
        mapper->setMapping(funcBtn[i], i);

        grid->addLayout(vbox, i / 5, i % 5);
    }
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(onFuncSelect(int)));
    grid->setVerticalSpacing(15);
    grid->setHorizontalSpacing(50);

    mainBtnStyleRender();

    logo = new QLabel;
    logo->setPixmap(QPixmap("../icons/mainwindow/logo.png"));

    title = new QLabel;
    title->setText("电 源 远 程 监 控 系 统");
    title->setAlignment(Qt::AlignCenter);

#ifdef ARM_COMPILE
    title->setStyleSheet("color:rgb(200,19,55)");
#else
    title->setStyleSheet("font-size: 30px;color:rgb(200,19,55)");
#endif

    QHBoxLayout *logoLayout = new QHBoxLayout;
    logoLayout->addSpacing(20);
    logoLayout->addWidget(logo);
    logoLayout->addSpacing(80);
    logoLayout->addWidget(title);
    logoLayout->addStretch();

    QHBoxLayout *funcLayout = new QHBoxLayout;
    funcLayout->addStretch();
    funcLayout->addLayout(grid);
    funcLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(logoLayout);
    mainLayout->addSpacing(80);
    mainLayout->addLayout(funcLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(mainLayout);
}

void MainWindow::onFuncSelect(int i)
{
    switch(i)
    {
    case PowerMon:
        {
//            mctrInfo->enableQuery();
            PowerMonitorDlg dlg;
            dlg.exec();
//            mctrInfo->disableQuery();
        }
        break;
    case Environment:
        break;
    case QueryData:
        {
            DataQueryDlg dlg;
            dlg.exec();
        }
        break;
    case QueryLog:
        {
            LogQueryDlg dlg;
            dlg.exec();
        }
        break;
    case Setup:
        {
            ParameterConfigDlg dlg;
            dlg.exec();
        }
        break;
    default:
        break;
    }
}

/**
* @brief 事件过滤器
* @param [In] 对象
* @param [In] 事件
* @return
*/
bool MainWindow::eventFilter(QObject *obj, QEvent *ev)
{
    Q_UNUSED(obj)
    Q_UNUSED(ev)
    return true;
}

/**
* @brief 上电初始化
*/
void MainWindow::powerOn()
{
    if (!powerOnForInit)
    {
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
void MainWindow::errorToRecovery(bool init)
{
    init = init;
}

/**
* @brief 获取按键名称
* @param [In] 序号
* @retval 按键名称
*/
QString MainWindow::getFuncName(int i)
{
    if (i < FunCount && i >= 0)
        return nameList.at(i);
    else
        return QString();
}

/**
* @brief 获取按键图标
* @param [In] 序号
* @retval 按键图标
*/
QPixmap MainWindow::getFuncIcon(int i)
{
    QString path = "../icons/mainwindow/";
    QPixmap icon;

    if (i < FunCount && i >= 0)
    {
        icon.load(path + iconList.at(i));
        return icon;
    }

    return QPixmap();
}

/**
* @brief 文本显示接口函数
* @note 多语言动态切换下需要retranslateUi()
*/
void MainWindow::retranslateUi()
{
//    defaultTitle();
//    setBtnName();
}

/**
* @brief changeEvent
* @note LanguageChange事件的处理函数，用以改变当前语言
*/
void MainWindow::changeEvent(QEvent *e)
{
    Q_UNUSED(e)
//    retranslateUi();
//    changeEvent(e);
}

/**
* @brief 按键风格渲染
*/
void MainWindow::mainBtnStyleRender()
{

}

/**
* @brief 更新背景色
*/
void MainWindow::updateBkColor()
{
//    QPalette plt = palette();
//    plt.setBrush(QPalette::Window, QBrush(QColor(114, 136, 175), Qt::Dense1Pattern));
//    setPalette(plt);
//
//    update();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QDialog::paintEvent(e);

    QPainter pp(this);
    pp.setPen(Qt::NoPen);
    pp.setBrush(QColor(182, 147, 139));
    pp.drawRect(QRect(10, 85, 780, 5));

    pp.setBrush(QBrush(QColor(114, 136, 175), Qt::Dense1Pattern));
    pp.drawRect(QRect(20, 100, 760, 300));
}
