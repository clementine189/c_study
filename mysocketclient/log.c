#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

#define DEBUG_FILE_ "socketclient.log"
#define MAX_STRING_LEN 10240

//LEVEL类别
#define NO_LOG_LEVEL    0
#define DEBUG_LEVEL     1
#define INFO_LEVEL      2
#define WARNING_LEVEL   3
#define ERROR_LEVEL     4

int LogLevel[5] = {
                    NO_LOG_LEVEL,
                    DEBUG_LEVEL,
                    INFO_LEVEL,
                    WARNING_LEVEL,
                    ERROR_LEVEL
};

//LEVEL的名称
char LevelName[5][10] = {
                        "NOLOG",
                        "DEBUG",
                        "INFO",
                        "WARNING",
                        "ERROR"
};

static int Error_GetCurTime(char* strTime)
{
    struct tm* tmTime = NULL;
    size_t timeLen = 0;
    time_t tTime = 0;

    tTime = time(NULL);
    tmTime = localtime(&tTime);
    timeLen = strfTime(strTime, 33, "%Y.%m.%d %H:%M:%S", tmTime);

    return timeLen;
}

static int Error_OpenFile(int* pf)
{
    char fileName[1024];
    memset(fileName, 0, sizeof(fileName));
    
    sprintf(fileName, "%s/log/%s", getenv("HOME"), DEBUG_FILE_);

    *pf = open(fileName, O_WRONLY|O_CREAT|O_APPEND, 0666);
    if(*pf < 0)
    {
        return -1;
    }

    return 0;
}
static void Error_Core(const char *file, int line, int level, int status, const char fmt, va_list args)
{
    char str[MAX_STRING_LEN];
    int strLen = 0;
    char tmpStr[64];
    int tmpStrLen = 0;
    int pf = 0;

    //初始化
    memset(str, 0, MAX_STRING_LEN);
    memset(tmpStr, 0, 64);

    //加入LOG时间
    tmpStrLen = Error_GetCurTime(tmpStr);
    tmpStrLen = sprintf(str, "[%s] ", tmpStr);
    strLen = tmpStrLen;

    //加入LOG等级
    tmpStrLen = sprintf(str+strLen, "[%s] ",LevelName[level]);
    strLen += tmpStrLen;

    //加入LOG状态
    
}