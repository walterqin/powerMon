/**
* @file parentWindow.h
* @brief 自定义父窗口类
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
#ifndef PARENTWINDOW_H
#define PARENTWINDOW_H

#include <QDialog>
#include "public/pushButton.h"
#include "public/toolButton.h"
#include "public/label.h"
#include "public/titleBar.h"
//#include "message/messageBox.h"
#include "configure/configure.h"
#include "configure/machineconfig.h"

/**
* @brief 程序状态
*/
//enum Status
//{
//        ExamStatus = 0,         /**< 成分检测 */
//        Cali,            /**< 成分项设置 */
//        SampleInput,         /**< 样本输入 */
//        ResultQuery,         /**< 历史查询 */
//        Configure,           /**< 配置窗体 */
//        Scale,               /**< 标定设置 */
//        DebugMode            /**< 调试模式 */
//};

enum {ItemNum = 10};

/**
* @class ParentWindow
* @brief 自定义父窗口抽象类定义
* @ingroup public
*/
class ParentWindow : public QDialog
{
	Q_OBJECT

public:
    ParentWindow(QWidget *parent = 0);
    virtual ~ParentWindow() { }

    void updateMainTip();
    void updateClock();

protected slots:
    void updateButtonTip();
    void noNeedUpdateTip();

protected:
    static bool powerOnForInit;       /**< 上电标志 */

protected:
    TitleBar *myTitleBar; /**< 自定义标题栏 */

    PushButton *createPushButton(const QSize &size, const QPixmap &pix, const QString &text,
										const char *member1, const char *member2);
    PushButton *createPushButton(const QSize &size, const QPixmap &pix, const QString &text, const char *member1);
    ToolButton *createToolButton(const QSize &size, const QPixmap &pix, const QString &text,
								  Qt::ArrowType type, const char *member1, const char *member2);
    ToolButton *createToolButton(const QSize &size, const QPixmap &pix, const QString &text,
                                                                  Qt::ArrowType type, const char *member);
};

#endif // PARENTWINDOW_H
