#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
 
 
#include "log_manager.h"
 
/*********************************************************/
 
 
/*常量****************************************************/
const char* ConfigText =
"[global]\n"
"strict init = true\n"
"buffer min = 1024\n"
"buffer max = 2048\n"
"file perms = 600\n"
"[formats]\n"
"simple	= \"%d.%-8.8us %-5V [%-8.8p.%-8.8t %F %L] %m%n\"\n"
"[rules]\n"
"my_cat.*		>stderr;\n"
"my_cat.INFO		\"info.txt\",10kb * 3 ~ \"info.txt.#r\";simple\n";
 
 
//优先级从低到高 debug info notice warn error fatal  debug大于等于debug的优先级都能给通过debug输出。
 
 
/*********************************************************/
static zlog_category_t * logHandler_m = NULL;
static int DetectConfigFile(const char *File);
 
/*****************************************************************************
 函 数 名  : LogMgr.LogMgr
 功能描述  : 日志模块初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 修改历史      :
  1.日    期   : 2012年12月19日 星期三
    作    者   : fensjoy
    修改内容   : 新生成函数
*****************************************************************************/
zlog_category_t * Init_zlog()
{
	DetectConfigFile(LOG_CONFIG);
 
	zlog_init(LOG_CONFIG);
    logHandler_m = zlog_get_category(LOG_CAT);
	if (!logHandler_m)
    {
		printf("%s in Init_zlog, get cat fail! line = %d\n",PRO_NAME, __LINE__);
		zlog_fini();
        remove(LOG_CONFIG);
		exit(-1);
	}
	return logHandler_m;
};
 
/*****************************************************************************
 函 数 名  : DetectConfigFile
 功能描述  : 检测日志模块的配置文件是否存在，不存在则创建
 输入参数  : const char *File
 输出参数  : 无
 返 回 值  : static
 修改历史      :
  1.日    期   : 2012年12月20日 星期四
    作    者   : fensjoy
    修改内容   : 新生成函数
*****************************************************************************/
static int DetectConfigFile(const char *File)
{
	FILE *fp  = NULL;
	int iRet = 0;
	DIR *log_p = NULL;
	char *cmd_buf = NULL;
 
	/*判断日志文件保存目录是否存在，不存在则创建*/
	log_p = opendir(LOG_PATH);
	if ( log_p == NULL )
	{
		printf("%s in DetectConfigFile, the log dir is not exist, now creat: %s!\n", PRO_NAME,LOG_PATH);
 
		cmd_buf = new char[64];
 
		printf("%s in DetectConfigFile, the log dir is not exist, now creat!\n", PRO_NAME);
		snprintf(cmd_buf, 64, "mkdir -p %s &", LOG_PATH);
		iRet = system(cmd_buf);
 
		delete [] cmd_buf;
 
	}
	else
	{
		closedir(log_p);
	}
 
	if ( access(File, F_OK) != 0 )
	{
		printf("%s log config file not exist,now creat!\n", PRO_NAME);
		fp = fopen(File, "w+");
		if(fp == NULL)
		{
			perror("#[db]: in DetectConfigFile, fopen config file error!");
			return -1;
		}
		iRet = fwrite(ConfigText, 1, strlen(ConfigText), fp);
		if ( iRet < 0 )
		{
			perror("#[para]: in DetectConfigFile, fwrite error!");
			fclose(fp);
			return -1;
		}
 
		fclose(fp);
	}
 
 
	return 0;
}
 
/*****************************************************************************
 函 数 名  : getZlogHandler
 功能描述  : 获取zlog日志句柄
 输入参数  : 无
 输出参数  : 无
 返 回 值  : zlog_category_t
 修改历史      :
  1.日    期   : 2014年1月24日 星期五
    作    者   : fensjoy
    修改内容   : 新生成函数
*****************************************************************************/
zlog_category_t *  getZlogHandler()
{
    if ( NULL == logHandler_m )
	{
		logHandler_m = Init_zlog();
	}
 
	return logHandler_m;
}
 