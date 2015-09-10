/**
* @file powermonitordlg.h
* @brief 电源状态监控窗口定义
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
#ifndef POWERMONITORDLG_H
#define POWERMONITORDLG_H

#include "../public/parentWindow.h"
#include <QSqlDatabase>

class QLineEdit;
class QGridLayout;
class QGroupBox;

/** @brief 电源监控类 */
class PowerMonitorDlg : public ParentWindow
{
    Q_OBJECT

public:
    PowerMonitorDlg(QWidget *parent = 0);

private slots:
    void onExit();
    void onOpenPower(int num);
    void onClosePower(int num);
    void onRenderPowerParamter();
    void onRenderRelayStatus();
    void onFlashText();
    void onGetPowerRelay();

private:
    void init();
    void createWidget();
    void setupUi();
    void defaultTitle();
    void renderStatusLamp(int lamp, bool flag);
    void paintEvent(QPaintEvent *ev);
    void getInitStatus();
    void getConfigInfo();

private:
    enum { MaxItem = 8};

    PushButton *openBtn[MaxItem];
    PushButton *closeBtn[MaxItem];
    PushButton *exitBtn;
    QLabel *voltageLbl[MaxItem];
    QLabel *currentLbl[MaxItem];
    QLabel *statusLbl[MaxItem];

    QLineEdit *voltageEdt[MaxItem];
    QLineEdit *currentEdt[MaxItem];
    QLabel *statusLamp[MaxItem];

    bool statusLampFlag[MaxItem];

    QGroupBox *switchType[MaxItem];

    QGridLayout *gridLayout;

private:
    int num;
    QStringList flag;
    QTimer *timer;
    QTimer *renderTimer;
    int vol[MaxItem];
    int cur[MaxItem];
    bool flash;
    QSqlDatabase db;
};

#endif // POWERMONITORDLG_H
