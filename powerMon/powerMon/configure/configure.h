/**
* @file  configure.h
* @brief 配置文件
* @author qgj
* @date 2015-06-04
*
* COPYRIGHT (C) 2015, 2016
*
* Change Logs:
* Date          Author        Notes
* 2015-06-04    QinGenjian    create
*
*/
#ifndef CONFIGURE_H
#define CONFIGURE_H


#define X_SIZE     800  /**< 标准对话框宽度 */
#define Y_SIZE     480  /**< 标准对话框高度 */


#define X86_COMPILE /**<  X86编辑标志 */

#ifndef X86_COMPILE
	#define ARM_COMPILE /**< ARM编辑标志 */
#endif

/** @brief 通讯开关 */
//#define COMTHREAD
/** @brief 通讯调试信息开关 */
//#define COM_DEBUG

#define MACHINE_POWERMONITOR

#ifdef X86_COMPILE
    #undef COMTHREAD
    #undef COM_DEBUG
    #undef RS485
#else
    #ifndef COMTHREAD
        #undef COM_DEBUG
    #else
        #define RS485
    #endif
#endif



#endif // CONFIGURE_H
