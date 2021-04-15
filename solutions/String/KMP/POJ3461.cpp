// difficulty: 1
// Oulipo
// 可重叠匹配次数，KMP典型应用
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e6 + 10;
char ms[maxn], ps[maxn];
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
int KmpMatch(char ms[], char ps[])
{
    int pLen = strlen(ps);
    int res = 0;
    for(int i = 0, j = 0; ms[i]; )
    {
        if(j == -1 || ms[i] == ps[j])
        {
            if(j == pLen - 1)
                res ++, j = nex[j];
            else
                ++ i, ++ j;
        }
        else
            j = nex[j];
    }
    return res;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%s%s", ps, ms);
        BuildNext(ms, nex);
        printf("%d\n", KmpMatch(ms, ps));
    }
    return 0;
}