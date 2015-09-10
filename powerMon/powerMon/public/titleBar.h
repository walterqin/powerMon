/**
* @file titleBar.h
* @brief 自定义标题栏
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
#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>

class QLabel;
class QTimer;
/**
* @class TitleBar
* @brief 自定义标题栏类定义
* @ingroup public
*/
class TitleBar : public QWidget
{
	Q_OBJECT

public:
    TitleBar(QWidget *parent = 0);

    void setMyWindowTitle(const QString &title);
    QString myWindowTitle() const;
    void disabelTimerDisp();
    void titleBarStyle();
    void updateWarnFlag(bool warn = false);

public slots:
    void updateTimeDisp();

signals:
    void unlockPanel();
    void openAlarmTip();
    void closeAlarmTip();

protected:
    void timerEvent(QTimerEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private slots:
    void receiveUnlock();

private:
    QLabel *iconLabel;    /**< 标题栏图标 */
    QLabel *titleLabel;    /**< 标题栏标题 */
    QLabel *timeLabel;   /**< 标题栏时钟 */
    QLabel *warnLabel;   /**< 警告提示 */

    bool dispTime;          /**< 时钟显示开关 */
    QTimer *unlockTimer;/**< 解锁定时器 */

    bool alarmTip;      /**< 是否显示警告提示对话框 */
};

#endif // TITLEBAR_H
