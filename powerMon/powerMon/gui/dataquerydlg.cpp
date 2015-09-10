/**
* @file dataquerydlg.cpp
* @brief 数据查询窗口类实现
* @author qgj
* @date 2015-06-15
* @ingroup gui
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-15    QinGenjian    create
*
*/
#include <QtGui>
#include <QSqlTableModel>
#include <QTableView>
#include "../database/profile.h"
#include "dataquerydlg.h"

/**
* @brief 构造函数
* @param [I] 父指针
*/
DataQueryDlg::DataQueryDlg(QWidget *parent) :
    ParentWindow(parent)
{
    init();
    setupUi();
}

/**
* @brief 初始化
*/
void DataQueryDlg::init()
{
}

/**
* @brief 创建窗体控件
*/
void DataQueryDlg::createWidget()
{
    tableModel = new QSqlTableModel(this, profile.dataDatabase());
    tableModel->setTable(QString("result"));
    tableModel->select();

    tableView = new QTableView;
    tableView->setModel(tableModel);
//    tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setColumnWidth(DataID, 50);
    tableView->setColumnWidth(DataVolt, 100);
    tableView->setColumnWidth(DataCurr, 100);
    tableView->setColumnWidth(DataAP, 100);
    tableView->setColumnWidth(DataTotal, 100);
    tableView->setColumnWidth(DataPF, 100);
    tableView->setColumnWidth(DataHZ, 100);
//    tableView->resizeColumnToContents(LogContent);
    tableView->setColumnWidth(DataDate, 130);
    tableView->setColumnWidth(DataTime, 100);
    tableView->verticalHeader()->setVisible(false);
    tableView->setFixedSize(780, 220);
    tableView->show();

    queryBtn = createPushButton(QSize(100, 50), QPixmap(), QString("查询"),
                                SLOT(updateButtonTip()), SLOT(onQuery()));
    sortBtn = createPushButton(QSize(100, 50), QPixmap(), QString("排序"),
                               SLOT(updateButtonTip()), SLOT(onSort()));
    exitBtn = createPushButton(QSize(100, 50), QPixmap(), QString("退出"),
                               SLOT(updateButtonTip()), SLOT(onExit()));
    cleanAllBtn = createPushButton(QSize(100, 50), QPixmap(), QString("清除记录"),
                                   SLOT(updateButtonTip()), SLOT(onCleanAll()));

    queryBtn->setObjectName(QString("查询"));
    sortBtn->setObjectName(QString("排序"));
    exitBtn->setObjectName(QString("退出"));
    cleanAllBtn->setObjectName(QString("清除记录"));

    dateGbx = new QGroupBox(tr("日期"));

    allDate = new QRadioButton(tr("全部"));
    allDate->setChecked(true);
    today = new  QRadioButton("今天");
    range = new QRadioButton("指定");

    begin = new QDateEdit(QDate::currentDate());
    end = new QDateEdit(QDate::currentDate());

    line = new QLabel;
    line->setText("-");

    levelCbx = new QCheckBox(tr("属性"));

    attrGbx = new QGroupBox(tr("属性"));
    for (int i = DataID; i < Count; i++)
    {
        attrCbx[i] = new QCheckBox(getAttrBtnName(i));
        attrVal[i] = new QComboBox;
        attrVal[i]->addItems(getAttrSelValue(i));
        if (i != DataWayNum && i != DataVolt && i != DataCurr)
        {
            attrCbx[i]->setVisible(false);
            attrVal[i]->setVisible(false);
        }
    }

    sortGbx = new QGroupBox(tr("排序"));
    dateRbtn = new QRadioButton(tr("日期"));
    waynumRbtn = new QRadioButton(tr("路号"));
    idRbtn = new QRadioButton(tr("ID"));
    idRbtn->setChecked(true);
}

/**
* @brief 窗体布局
*/
void DataQueryDlg::setupUi()
{
    defaultTitle();
    createWidget();

    QHBoxLayout *logLayout = new QHBoxLayout;
    logLayout->addWidget(tableView, 0, Qt::AlignHCenter);

    QGridLayout *attrGrid = new QGridLayout;
    for (int i = DataID; i < Count; i++)
    {
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget(attrCbx[i]);
        layout->addWidget(attrVal[i]);
        attrGrid->addLayout(layout, i, 0);
    }

    attrGbx->setLayout(attrGrid);

    QVBoxLayout *drLayout = new QVBoxLayout;
    drLayout->addWidget(begin);
    drLayout->addWidget(end);

    QHBoxLayout *rangeLayout = new QHBoxLayout;
    rangeLayout->addWidget(range);
    rangeLayout->addStretch();
    rangeLayout->addLayout(drLayout);

    QVBoxLayout *dateLayout = new QVBoxLayout;
    dateLayout->addWidget(allDate);
    dateLayout->addWidget(today);
    dateLayout->addLayout(rangeLayout);
    dateGbx->setLayout(dateLayout);

    QHBoxLayout *levelLayout = new QHBoxLayout;
    levelLayout->addWidget(levelCbx);
    levelLayout->addStretch();

    QVBoxLayout *sortLayout = new QVBoxLayout;
    sortLayout->addWidget(idRbtn);
    sortLayout->addWidget(waynumRbtn);
    sortLayout->addWidget(dateRbtn);
    sortGbx->setLayout(sortLayout);

    QHBoxLayout *condLayout = new QHBoxLayout;
    condLayout->addWidget(dateGbx);
    condLayout->addWidget(attrGbx);
    condLayout->addWidget(sortGbx);

    QHBoxLayout *keyLayout1 = new QHBoxLayout;
    keyLayout1->addWidget(queryBtn);
    keyLayout1->addWidget(sortBtn);

    QHBoxLayout *keyLayout2 = new QHBoxLayout;
    keyLayout2->addWidget(cleanAllBtn);
    keyLayout2->addWidget(exitBtn);

    QVBoxLayout *keyLayout = new QVBoxLayout;
    keyLayout->addLayout(keyLayout1);
    keyLayout->addLayout(keyLayout2);

    QHBoxLayout *funcLayout = new QHBoxLayout;
    funcLayout->addSpacing(20);
    funcLayout->addLayout(condLayout);
    funcLayout->addStretch();
    funcLayout->addLayout(keyLayout);
    funcLayout->addSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(myTitleBar);
    mainLayout->setMargin(0);
    mainLayout->addLayout(logLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(funcLayout);
    mainLayout->addStretch();
    mainLayout->setContentsMargins(0, 0, 5, 5);

    setLayout(mainLayout);
}

/**
* @brief 窗口标题
*/
void DataQueryDlg::defaultTitle()
{
    myTitleBar->setMyWindowTitle(tr("数据结果查询"));
}

/**
* @brief 查询键槽函数
*/
void DataQueryDlg::onQuery()
{
    QString strDate;
    QString startDate, stopDate;
    QString strSql;

    if (today->isChecked())
    {
        strDate = QDate::currentDate().toString("yyyy-MM-dd");
        strSql = QString("date = '") + strDate + QString("' and ");
    }
    else if (range->isChecked())
    {
        startDate = begin->text();
        stopDate = end->text();
        strSql = QString("date >= '") + startDate + QString("' and date <='") + stopDate + QString("' and ");
    }

    QString vAlarmUp = profile.value("vAlarmUp").toStringList().at(0).split("V").at(0);
    QString vAlarmLow = profile.value("vAlarmLow").toStringList().at(0).split("V").at(0);
    QString iAlarmUp = profile.value("iAlarmUp").toStringList().at(0).split("A").at(0);
    QString iAlarmLow = profile.value("iAlarmLow").toStringList().at(0).split("A").at(0);

    QStringList strCond;
    strCond.clear();
    for (int i = DataID; i < Count; i++ )
    {
        QString tmp;
        tmp.clear();
        if (attrCbx[i]->isChecked())
        {
            if ( i == DataWayNum)
            {
                tmp = QString("waynum='") + attrVal[i]->currentText() + QString("'");
                strCond << tmp;
            }
            else if (i == DataVolt)
            {
                if (attrVal[i]->currentText() == QString("高于上限"))
                {
                    tmp = QString("volt > '") + vAlarmUp + QString("'");
                    strCond << tmp;
                }
                else if (attrVal[i]->currentText() == QString("低于下限"))
                {
                    tmp = QString("volt < '") + vAlarmLow + QString("'");
                    strCond << tmp;
                }
                else
                {
                    tmp = QString("volt >= '") + vAlarmLow + QString("'") + QString(" and ")
                          + QString("volt <= '") + vAlarmUp + QString("'");
                    strCond << tmp;
                }
            }
            else if (i == DataCurr)
            {
                if (attrVal[i]->currentText() == QString("高于上限"))
                {
                    tmp = QString("curr > '") + iAlarmUp + QString("'");
                    strCond << tmp;
                }
                else if (attrVal[i]->currentText() == QString("低于下限"))
                {
                    tmp = QString("curr < '") + iAlarmLow + QString("'");
                    strCond << tmp;
                }
                else
                {
                    tmp = QString("curr >= '") + iAlarmLow + QString("'") + QString(" and ")
                          + QString("curr <= '") + iAlarmUp + QString("'");
                    strCond << tmp;
                }
            }
        }
    }


    if (!strCond.isEmpty())
    {
        for (int i = 0; i < strCond.count(); i++)
        {
            strSql += strCond.at(i);
            strSql += QString(" and ");
        }
        strSql.chop(5);
    }
    else if (!strSql.isEmpty())
            strSql.chop(5);

    tableModel->setFilter(strSql);
    tableModel->select();
}

/**
* @brief 排序键槽函数
*/
void DataQueryDlg::onSort()
{
    int col = DataID;

    if (idRbtn->isChecked())
        col = DataID;
    else if (waynumRbtn->isChecked())
        col = DataWayNum;
    else
        col = DataDate;
    tableModel->setSort(col, Qt::AscendingOrder);
    tableModel->select();
}

/**
* @brief 退出键槽函数
*/
void DataQueryDlg::onExit()
{
    close();
}

/**
* @brief 清除全部数据槽函数
*/
void DataQueryDlg::onCleanAll()
{

}

QString DataQueryDlg::getAttrBtnName(int i)
{
    QStringList list;
    list << tr("ID") << tr("路号") << tr("电压") << tr("电流") << tr("有功功率") << tr("总功率") << tr("功率因数") << tr("频率")
            << tr("日期") << tr("时间");
    return list.at(i);
}

QStringList DataQueryDlg::getAttrSelValue(int i)
{
    QStringList ret;

    if (i > DataTime || i < DataID)
        return QStringList();

    if (i == DataWayNum)
        ret << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8";
    else if (i == DataVolt || i == DataCurr)
        ret << QString("正常") << QString("高于上限") << QString("低于下限");
    else
        ret << QString("全部");

    return ret;
}
