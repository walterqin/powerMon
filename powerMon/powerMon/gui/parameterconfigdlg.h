/**
* @file parameterconfigdlg.h
* @brief 参数设置窗口类定义
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
#ifndef PARAMETERCONFIGDLG_H
#define PARAMETERCONFIGDLG_H

#include "../public/parentWindow.h"
#include "paraminputdialog.h"

class ParameterConfigDlg : public ParentWindow
{
Q_OBJECT
public:
    explicit ParameterConfigDlg(QWidget *parent = 0);

signals:

public slots:
//    void onExit();
    void onLeftPage();
    void onRightPage();
    void onDealItemParams(int i);

protected:
    void changeEvent(QEvent *e);

private:
    void init();
    void createWidget();
    void setupUi();

    void updateCurPage();
    void retranslateUi();
    void inputValueRender(QString item, int i);
    void defaultTitle();

private:
    enum
    {
        ItemsPerPage = 6                    /**< 每页显示的最大条目数 */
    };

    int m_nCount;                           /**< 当前窗口下的要显示的参数个数*/
    int m_nPage;                            /**< 当前是第几页 */
    int m_nPages;                           /**< 总页数 */

    QLabel *m_PageLabel;                    /**< 当前第几页的显示处*/
    PushButton *m_Buttons[ItemsPerPage];    /**< 显示的按钮 共ItemsPerPage个*/
    QLabel *m_Tips[ItemsPerPage];           /**< 相应参数的提示信息*/
    QLabel *m_Labels[ItemsPerPage];         /**< 相应参数的当前值*/
    PushButton *m_RetButton;                /**< 返回按钮*/
    ToolButton *m_LeftButton;               /**< 左翻页按钮*/
    ToolButton *m_RightButton;              /**< 右翻页按钮*/
    QLabel *m_lblTitle;                     /**< 子标题显示控件*/
    const char *m_pTitle;                   /**< 子标题显示串*/
    ParamInputDialog *input;

    QStringList paramKeys;                   /**< 参数项 */
    QStringList value;
    QStringList tip;
    QStringList editType;
};

#endif // PARAMETERCONFIGDLG_H
