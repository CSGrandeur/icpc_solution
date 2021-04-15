// difficulty: 2
// Power Strings
// sLen - nex[sLen] 即循环节长度，判断是否铺满字符串（长度整除）

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e6 + 10;
char s[maxn];
int nex[maxn];
void BuildNext(char st[], int nex[])
{
    nex[0] = -1;
    for(int i = 0, j = -1; st[i]; )
    {
        if(j == -1 || st[i] == st[j])
            nex[++ i] = ++ j;
        else
            j = nex[j];
    }
}
int main()
{
    while(scanf("%s", s) && s[0] != '.')
    {
        BuildNext(s, nex);
        int sLen = strlen(s);
        printf("%d\n", sLen % (sLen - nex[sLen]) ? 1 : sLen / (sLen - nex[sLen]));
    }
    return 0;
}