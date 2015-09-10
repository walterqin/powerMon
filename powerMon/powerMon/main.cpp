/**
* @file  main.cpp
* @brief 主函数文件
*<pre><b>北京东方瑞威科技发展股份有限公司</b></pre>
* @author qgj
* @date 2015-06-03
* <pre>qingenjian10@163.com</pre>
*/

#include <QtGui>
#include <QWSServer>
#include <QTextCodec>
#include <QTranslator>
#include "configure/configure.h"
#include "public/statManager.h"
#include "commu/commanager.h"
#include "commu/cominfo.h"
#include "public/buttoneventfilter.h"
#include "gui/mainwindow.h"

/** @brief 定义添加应用程序启动画面 */
#define SPLASH_SCREEN

QTranslator translator;         /**< 翻译器对象 */
StatManager *statMgr = NULL;    /**< 外设对象 */
ComManager *comManager = NULL;       /**< 通讯线程对象 */
ComInfo *mctrInfo = NULL;                         /**< 主控信息对象 */

/**
* @brief 主函数
* @param[in] argc为命令行参数个数
* @param[in] argv为命令行全部参数
* @return 返回执行结果
*/
int main(int argc, char **argv)
{
    QApplication app(argc, argv, QApplication::GuiServer);

    QWSServer::setCursorVisible(false);
    QWSServer::setBackground(QBrush(Qt::black));

#ifdef SPLASH_SCREEN
    QSplashScreen splash(QPixmap("../pic/wait.png"));
    splash.show();
    app.processEvents();
#endif

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));


#ifdef ARM_COMPILE
    QFont font;
    font.setFamily("wenquanyi");
    font.setPointSize(160);
    font.setBold(true);
    app.setFont(font);
//    app.setFont(QFont("wqy-micro.ttf", 20));
#endif

    app.installTranslator(&translator);

    BackLightMgr::open();
    AppEventFilter *evFilter = new AppEventFilter();
    comManager = new ComManager;
    mctrInfo = new ComInfo;
    statMgr = new StatManager;
    statMgr->panelStyle();

    //TODO:添加窗体对象
    MainWindow *pWin = new MainWindow;
    pWin->show();
    pWin->updateBkColor();

#ifdef SPLASH_SCREEN
    splash.finish(pWin);
#endif
    app.processEvents();
    app.installEventFilter(evFilter);

    return app.exec();
}
