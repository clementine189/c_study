//#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cfg_op.h"
//每一行的最大长度
#define MaxLine 2048

// int isExist(char *lineBuf, char *pKey)
// {
//     char *pTmp = NULL, *pTmp2 = NULL;
//     pTmp = strchr(lineBuf, '=');
//     pTmp2 = strstr(lineBuf, pKey);
//     if(pTmp == NULL || pTmp2 ==NULL)
//         return -1;

//     for(char *c = lineBuf; c < pTmp2; c++)
//     {
//         if(*c != ' ')
//             return -1;
//     }
//     for(char *c = (pTmp2 + sizeof(pKey)); c < pTmp; c++)
//     {
//         if(*c != ' ')
//             return -1;
//     }

//     return 1;
// }
//获取配置项
//传入要查询的键（pKey），输出值（pValue）和长度（pValueLen）
//ps：如果同一个键被定义多次，会返回最后面的那一个
int GetCfgItem(char *pFileName, char *pKey, char *pValue, int *pValueLen)
{
    int ret = 0, flag = 0;
    FILE *fp = NULL;
    char *pTmp = NULL, *pTmp2 = NULL, *pFind = NULL, *pBegin = NULL, *pEnd = NULL;
    
    char lineBuf[MaxLine];

    fp = fopen(pFileName, "r");
    if (fp == NULL)
    {
        ret = -1;
        return ret;
    }

    while(!feof(fp))
    {
        memset(lineBuf, 0, sizeof(lineBuf));
        fgets(lineBuf, MaxLine, fp);
        pTmp = strchr(lineBuf, '=');
        if(pTmp == NULL)
        {
            continue;
        }
        
        //查找pkey出现的位置
        pTmp = strstr(lineBuf, pKey);
        if(pTmp == NULL)
        {
            continue;
        }

        for(char *c = lineBuf; c < pTmp; c++)
        {
            if(*c != ' ')
            {
                flag = 1;
                break;
            }
        }
        if(flag == 1)
        {
            flag = 0;
            continue;
        }

        pTmp = pTmp + strlen(pKey);
        pTmp2 = strchr(pTmp, '=');
        if(pTmp2 == NULL)
        {
            continue;//没有等号
        }


        for(char *c = pTmp; c < pTmp2; c++)
        {
            if(*c != ' ')
            {
                pTmp2 = NULL;
            }
        }
        if(pTmp2 == NULL)
        {
            continue;//没有等号
        }


        pTmp = pTmp2 + 1;
  
       //获取value的起点
        while(1)
        {
            if(*pTmp == ' ')
            {
                pTmp ++;
            }
            else
            {
                pBegin = pTmp;
                if(*pBegin == '\n')
                {
                    //没有配置值
                    goto End;
                }
                break;
            }
        }

        //获取value的终点
        while(1)
        {
            if((*pTmp == ' ' || *pTmp == '\n'))
            {
                break;
            }
            else
            {
                pTmp ++;
            }
        }

        pEnd = pTmp;
        *pValueLen = pEnd - pBegin;
        memcpy(pValue, pBegin, (size_t)(pEnd - pBegin));
    }

End:
    if(fp == NULL)
    {
        fclose(fp);
    }

    return 0;
}


//写配置项
//循环读取每一行，检查key是否存在，若存在则修改对应的value值
//若不存在，在文件末尾添加"key = value"
int WriteCfgItem(char *pFileName /*in*/, char *pKey /*in*/, char *pValue /*in*/, int ValueLen /*in*/)
{
    int rv = 0, iTag = 0, length = 0, flag = 0;
    char lineBuf[MaxLine];
    FILE *fp = NULL;
    char *pTmp = NULL, *pTmp2 = NULL;
    char fileBuf[1024*8] = {0};


    if(pFileName == NULL || pKey == NULL || pValue == NULL)
    {
        rv = -1;
        printf("SetCfgItem() err. param err \n");
        goto End;
    }

    fp = fopen(pFileName, "r+");
    if(fp == NULL)
    {
        rv = -2;
        printf("fopen() err.\n");
    }

    if(fp == NULL)
    {
        fp = fopen(pFileName, "w+t");
        if(fp == NULL)
        {
            rv = -3;
            printf("fopen() err.\n");
            goto End;
        }
    }

    fseek(fp, 0L, SEEK_END);//把文件指针从0位置开始，移动到文件末尾
    length = ftell(fp);//获取文件长度

    fseek(fp, 0L, SEEK_SET);

    if(length > 1024 * 8)
    {
        rv = -3;
        printf("文件超过1024*8\n");
        goto End;
    }

    while(!feof(fp))
    {
        memset(lineBuf, 0, sizeof(lineBuf));
        fgets(lineBuf, MaxLine, fp);
        pTmp = strchr(lineBuf, '=');
        if(pTmp == NULL)
        {
            break;
        }

        pTmp2 = strstr(lineBuf, pKey);
        if(pTmp2 != NULL)
        {
            for(char *c = lineBuf; c < pTmp2; c++)
            {
                if(*c != ' ')
                {
                    flag = 1;
                    break;
                }
            }

            if(flag == 0 && pTmp2 + sizeof(pKey) != NULL)
            {
                for(char *c = pTmp2 + sizeof(pKey); c < pTmp; c++)
                {
                    if(*c != ' ')
                    {
                        flag = 1;
                        break;
                    }
                }

            }
        }


        if(flag == 1 || pTmp2 == NULL )
        {
            strcat(fileBuf, lineBuf);
            continue;
        }
        else
        {
            sprintf(lineBuf, "%s = %s\n", pKey, pValue);
            strcat(fileBuf, lineBuf);
            iTag = 1;
        }
        flag = 0;
    }
     
     
     if(iTag == 0)//若key关键字不存在，追加
     {
         fprintf(fp, "%s = %s \n", pKey, pValue);
     }
     else//若key关键字存在，重新创建文件
     {
         if(fp != NULL)
         {
             fclose(fp);
             fp = NULL;
         }

         fp = fopen(pFileName, "w+t");
         if(fp == NULL)
         {
             rv = -4;
            printf("fopen() err. \n");
            goto End;
         }
         fputs(fileBuf, fp);
     }
    End:
        if(fp != NULL)
        {
            fclose(fp);
        }
        return rv;
}