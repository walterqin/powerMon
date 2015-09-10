/**
* @file logquerydlg.h
* @brief 报警日志查询窗口类定义
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
#ifndef LOGQUERYDLG_H
#define LOGQUERYDLG_H

#include "../public/parentWindow.h"

class QTableView;
class QSqlTableModel;
class QGroupBox;
class QRadioButton;
class QDateEdit;
class QCheckBox;
class QComboBox;

class LogQueryDlg : public ParentWindow
{
Q_OBJECT
public:
    explicit LogQueryDlg(QWidget *parent = 0);

    enum {
        LogID = 0,
        LogLevel,
        LogContent,
        LogDate,
        LogCount,
        Count
    };

    enum
    {
        LevelDebug = 0,
        LevelWarn,
        LevelFatal,
    };

    enum
    {
        ColId = 0,
        ColLevel,
        ColContent,
        ColDate,
        ColCount,
        ColMax,
    };

signals:

public slots:
    void onQuery();
    void onSort();
    void onExit();
    void onCleanAll();

private:
    void init();
    void createWidget();
    void setupUi();
    void defaultTitle();

private:
    PushButton *queryBtn;
    PushButton *sortBtn;
    PushButton *exitBtn;
    PushButton *cleanAllBtn;

    QTableView *tableView;
    QSqlTableModel *tableModel;

    QGroupBox *dateGbx;
    QRadioButton *allDate;
    QRadioButton *today;
    QRadioButton *range;
    QDateEdit *begin, *end;
    QLabel *line;

    QGroupBox *levelGbx;
    QCheckBox *levelCbx;
    QComboBox *valCmb;
    QStringList levelVal;

    QString filter;

    QGroupBox *sortGbx;
    QRadioButton *dateRbtn;
    QRadioButton *levelRbtn;
    QRadioButton *idRbtn;
    QRadioButton *cntRbtn;
};

#endif // LOGQUERYDLG_H
