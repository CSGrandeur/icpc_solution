/*
字符串哈希模板题。当然又是KMP更直观。
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef unsigned long long ULL;
const int MAXN = 1000010;
const int BASE = 29;
ULL hashV[MAXN], pre[MAXN];
char buf[MAXN];
int main()
{
    pre[0] = 1;
    for(int i = 1; i < MAXN; i ++)
        pre[i] = pre[i - 1] * BASE;
    while(scanf("%s", buf) != EOF && buf[0] != '.')
    {
        hashV[0] = buf[0] - 'a';
        for(int i = 1; buf[i]; i ++)
            hashV[i] = hashV[i - 1] * BASE + buf[i] - 'a';
        int baseLen, slen = strlen(buf);
        for(baseLen = 1; baseLen <= slen / 2; baseLen ++)
        {
            if(slen % baseLen)
                continue;
            int j;
            for(j = baseLen * 2 - 1; j < slen; j += baseLen)
            {
                ULL jKey = hashV[j] - hashV[j - baseLen] * pre[baseLen];
                if(jKey != hashV[baseLen - 1])
                    break;
            }
            if(j >= slen)
                break;
        }
        printf("%d\n", baseLen <= slen / 2 ? slen / baseLen : 1);
    }
    return 0;
}