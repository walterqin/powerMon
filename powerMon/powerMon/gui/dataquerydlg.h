/**
* @file dataquerydlg.h
* @brief 数据查询窗口类定义
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
#ifndef DATAQUERYDLG_H
#define DATAQUERYDLG_H

#include "../public/parentWindow.h"

class QTableView;
class QSqlTableModel;
class QGroupBox;
class QRadioButton;
class QDateEdit;
class QComboBox;
class QCheckBox;

class DataQueryDlg : public ParentWindow
{
Q_OBJECT
public:
    explicit DataQueryDlg(QWidget *parent = 0);

    enum {
        DataID = 0,
        DataWayNum,
        DataVolt,
        DataCurr,
        DataAP,
        DataTotal,
        DataPF,
        DataHZ,
        DataDate,
        DataTime,
        Count
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
    QString getAttrBtnName(int i);
    QStringList getAttrSelValue(int i);

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
    QCheckBox *levelCbx;

    QGroupBox *attrGbx;
    QCheckBox *attrCbx[Count];
    QComboBox *attrVal[Count];

    QGroupBox *sortGbx;
    QRadioButton *dateRbtn;
    QRadioButton *waynumRbtn;
    QRadioButton *idRbtn;

};

#endif // LOGQUERYDLG_H
