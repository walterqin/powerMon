/**
* @file logquerydlg.cpp
* @brief 报警日志查询窗口类实现
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
#include "logquerydlg.h"

/**
* @brief 构造函数
* @param [I] 父指针
*/
LogQueryDlg::LogQueryDlg(QWidget *parent) :
    ParentWindow(parent)
{
    init();
    setupUi();
}

/**
* @brief 初始化
*/
void LogQueryDlg::init()
{
    filter.clear();
    levelVal << "debug" << "warn" << "fatal";
}

/**
* @brief 创建窗体控件
*/
void LogQueryDlg::createWidget()
{
    tableModel = new QSqlTableModel(this, profile.dataDatabase());
    tableModel->setTable(QString("log"));
    tableModel->select();

    tableView = new QTableView;
    tableView->setModel(tableModel);
//    tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setColumnWidth(LogID, 50);
    tableView->setColumnWidth(LogLevel, 100);
    tableView->setColumnWidth(LogContent, 400);
//    tableView->resizeColumnToContents(LogContent);
    tableView->setColumnWidth(LogDate, 130);
    tableView->setColumnWidth(LogCount, 70);
    tableView->verticalHeader()->setVisible(false);
    tableView->setFixedSize(780, 250);
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

    levelGbx = new QGroupBox(tr("级别"));
    levelCbx = new QCheckBox(tr("按级别"));
    valCmb = new QComboBox;
    valCmb->addItems(levelVal);

    sortGbx = new QGroupBox(tr("排序"));
    dateRbtn = new QRadioButton(tr("日期"));
    levelRbtn = new QRadioButton(tr("级别"));
    idRbtn = new QRadioButton(tr("ID"));
    cntRbtn = new QRadioButton(tr("次数"));
    idRbtn->setChecked(true);
}

/**
* @brief 窗体布局
*/
void LogQueryDlg::setupUi()
{
    defaultTitle();
    createWidget();

    QHBoxLayout *logLayout = new QHBoxLayout;
    logLayout->addWidget(tableView, 0, Qt::AlignHCenter);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(begin);
    vLayout->addWidget(end);

    QHBoxLayout *rangeLayout = new QHBoxLayout;
    rangeLayout->addWidget(range);
    rangeLayout->addStretch();
    rangeLayout->addLayout(vLayout);

    QVBoxLayout *dateLayout = new QVBoxLayout;
    dateLayout->addWidget(allDate);
    dateLayout->addWidget(today);
    dateLayout->addLayout(rangeLayout);
    dateGbx->setLayout(dateLayout);

    QHBoxLayout *lhLayout = new QHBoxLayout;
    lhLayout->addWidget(levelCbx);
    lhLayout->addWidget(valCmb);

    QVBoxLayout *levelLayout = new QVBoxLayout;
    levelLayout->addLayout(lhLayout);
    levelLayout->addStretch();
    levelGbx->setLayout(levelLayout);

    QVBoxLayout *sortLayout = new QVBoxLayout;
    sortLayout->addWidget(idRbtn);
    sortLayout->addWidget(levelRbtn);
    sortLayout->addWidget(dateRbtn);
    sortLayout->addWidget(cntRbtn);
    sortGbx->setLayout(sortLayout);

    QHBoxLayout *condLayout = new QHBoxLayout;
    condLayout->addWidget(dateGbx);
    condLayout->addWidget(levelGbx);
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

    setLayout(mainLayout);
}

/**
* @brief 窗口标题
*/
void LogQueryDlg::defaultTitle()
{
    myTitleBar->setMyWindowTitle(tr("日志查询"));
}

/**
* @brief 查询键槽函数
*/
void LogQueryDlg::onQuery()
{
    QString strDate;
    QString startDate, stopDate;
    QString lel;
    QString strSql;

    if (levelCbx->isChecked())
    {
        lel = valCmb->currentText();
    }

    if (today->isChecked())
    {
        strDate = QDate::currentDate().toString("yyyy-MM-dd");
        strSql = QString("日期 = '") + strDate + QString("' and ");
    }
    else if (range->isChecked())
    {
        startDate = begin->text();
        stopDate = end->text();
        strSql = QString("日期 >= '") + startDate + QString("' and 日期 <='") + stopDate + QString("' and ");
    }

    if (!lel.isEmpty())
        strSql += QString("级别 = '")  + lel + QString("'");
    else if (!strSql.isEmpty())
        strSql.chop(5);

    tableModel->setFilter(strSql);
    tableModel->select();
}

/**
* @brief 排序键槽函数
*/
void LogQueryDlg::onSort()
{
    int col = ColId ;

    if (idRbtn->isChecked())
        col = ColId ;
    else if (levelRbtn->isChecked())
        col = ColLevel;
    else if (cntRbtn->isChecked())
        col = ColCount;
    else
        col = ColDate;
    tableModel->setSort(col, Qt::AscendingOrder);
    tableModel->select();
}

/**
* @brief 退出键槽函数
*/
void LogQueryDlg::onExit()
{
    close();
}

/**
* @brief 清除全部数据槽函数
*/
void LogQueryDlg::onCleanAll()
{

}
