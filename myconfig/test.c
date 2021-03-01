#include <stdio.h>
#include "cfg_op.h"

#define CFGNAME "./mycfg.conf"
int main()
{
    int ret = 0;
    char name[1024] = {"happy"};
    char value[1024] = {0};
    int vlen = 0;

    ret = GetCfgItem(CFGNAME, name, value, &vlen);

    printf("key: %s\nvalue: %s\nvlen: %d\n", name, value, vlen);

    WriteCfgItem(CFGNAME, "me", "happy", 5);


    return 0;    
}