/**
* @file mainWindow.h
* @brief 主窗口定义
* @author qgj
* @date 2015-06-04
* @ingroup gui
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

#include "../public/parentWindow.h"

class QVBoxLayout;

/**
* @brief 主界面id定义
*/
enum WindowID
{
    PowerMon = 0,   /**< 电源检测 */
    Environment,    /**< 环境状态 */
    QueryData,      /**< 查询数据 */
    QueryLog,       /**< 查询日志 */
    Setup,          /**< 设置 */
    FunCount        /**< 功能总数 */
};

/**
* @brief 主窗口类定义
*/
class MainWindow : public ParentWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow() {}

    static void setObjectPointer(WindowID id, MainWindow *pWin);
    static MainWindow *getObjectPointer(WindowID id);

    void updateBkColor();

private slots:
    void onFuncSelect(int i);

private:
    void setupMainWindow();
    void init();
    void powerOn();
    void initSysClock();
    void reportWarnInit();
    void errorToRecovery(bool init = false);

    QString getFuncName(int i);
    QPixmap getFuncIcon(int i);

private:
    void mainBtnStyleRender();
    bool eventFilter(QObject *obj, QEvent *ev);
    void paintEvent(QPaintEvent *);
    void retranslateUi();
    void changeEvent(QEvent *e);

private:
    PushButton  *funcBtn[FunCount];
    Label       *funcLbl[FunCount];
    QButtonGroup *btnGroup;             /**< 选择按键组 */

    QStringList nameList;
    QStringList iconList;

    QLabel *logo;
    QLabel *title;

    static bool powerOnForInit;         /**< 上电标志 */

};

#endif // MAINWINDOW_H
