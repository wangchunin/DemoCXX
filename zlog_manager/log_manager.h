/******************************************************************************
                  版权所有 (C), 2001-2014, 
 ******************************************************************************
  文 件 名   : log_manger.h
  版 本 号   : 初稿
  作    者   : fensjoy
  生成日期   : 2013年6月5日 星期三
  最近修改   :
  功能描述   : log_manger.cpp 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2013年6月5日 星期三
    作    者   : fensjoy
    修改内容   : 创建文件
******************************************************************************/
#ifndef __LOG_MANGER_H__
#define ___LOG_MANGER_H__
#include <iostream>
#include "zlog.h"
#define PRO_NAME "#[db]: "
#define LOG_CONFIG "zlog.conf"
#define LOG_PATH   "./"
#define LOG_CAT    "my_cat"  //注意这里要与配置文件中保持一致，否则无法输出

//extern zlog_category_t * logHandler_m;
zlog_category_t * Init_zlog();
zlog_category_t *  getZlogHandler();
zlog_category_t *log_c =  Init_zlog();
#define   log_fatal(arg...)     zlog_fatal(log_c,##arg)
#define   log_notice(arg...)    zlog_notice(log_c,##arg)
#define   log_info(arg...)      zlog_info(log_c,##arg)
#define   log_debug(arg...)     zlog_debug(log_c,##arg)
#define   log_warn(arg...)      zlog_warn(log_c,##arg)
#define   log_error(arg...)     zlog_error(log_c,##arg)
#endif /* __LOG_MANGER_H__ */