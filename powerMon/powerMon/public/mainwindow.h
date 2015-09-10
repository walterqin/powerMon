/**
* @file mainWindow.h
* @brief 虚主窗口定义
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "parentWindow.h"

class QVBoxLayout;

/**
* @brief 虚主窗口类定义
* @ingroup public
*/
class MainWindow : public ParentWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() {}

    /**
    * @brief 主界面id定义
    */
    enum WindowID
    {
        Assay = 0,      /**< 样本检测 */
        Calibrate,      /**< 定标 */
        Curve,          /**< 曲线 */
        QualityControl, /**< 质量控制 */
        Setup,          /**< 设置 */
        Maintain,       /**< 调试 */
        Query,          /**< 查询 */
        Information     /**< 患者信息录入 */
    };

    static void setObjectPointer(WindowID id, MainWindow *pWin);
    static MainWindow *getObjectPointer(WindowID id);

    virtual void init();

protected slots:
    void onAssay();
    void onCalibrate();
    void onQc();
    void onQuery();
    void onMaintain();
    void onSetup();
    void onInformation();
    void onCurve();

protected:
    PushButton *assayBtn;
    PushButton *calibrateBtn;
    PushButton *qcBtn;
    PushButton *maintainBtn;
    PushButton *setupBtn;
    PushButton *queryBtn;
    PushButton *infoBtn;
    PushButton *curcleBtn;

    QVBoxLayout *rightLayout;     /**< 右侧布局 */

    static bool powerOnForInit;   /**< 上电标志 */

protected:
    void rightZoneLayout();
    void setFuncBtnName();
    void powerOn();

    QPixmap getShapeIcon(int i, bool small = true);
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    void initSysClock();
    void reportWarnInit();
    void errorToRecovery(bool init = false);
    void mainBtnStyleRender();
};

#endif // MAINWINDOW_H
