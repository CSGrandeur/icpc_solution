// KMP查找首次出现位置，字符串改成int来考察
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int mainNum[1100000], tmpNum[11000];
int next[11000];
int t, slen, tlen;
void BuildNext(int s[])
{
    next[0] = -1;
    for(int i = 0, j = -1; i < tlen; )
    {
        if(j == -1 || s[i] == s[j])
            next[++i] = ++j;
        else
            j = next[j];
    }
}
int KmpMatch(int ms[], int ts[])
{
    for(int i = 0, j = 0; i < slen;)
    {
        if(j == -1 || ms[i] == ts[j])
        {
            if(j == tlen - 1)
                return i - tlen + 1;
            else
                i ++, j ++;
        }
        else
            j = next[j];
    }
    return -2;
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        scanf("%d%d", &slen, &tlen);
        for(int i = 0; i < slen; i ++)
            scanf("%d", &mainNum[i]);
        for(int i = 0; i < tlen; i ++)
            scanf("%d", &tmpNum[i]);
        BuildNext(tmpNum);
        printf("%d\n", KmpMatch(mainNum, tmpNum) + 1);
    }
    return 0;
}