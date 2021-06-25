// difficulty: 1
// Oulipo
// 还是KMP统计出现次数
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
char mainStr[1100000], patStr[11000];
int next[11000];
void BuildNext(char ts[])
{
    next[0] = -1;
    for(int i = 0, j = -1; ts[i]; )
    {
        if(j == -1 || ts[i] == ts[j])
            next[++i] = ++j;
        else
            j = next[j];
    }
}
int KmpMatch(char ms[], char ts[])
{
    int tlen = strlen(ts);
    int ans = 0;
    for(int i = 0, j = 0; ms[i];)
    {
        if(j == -1 || ms[i] == ts[j])
        {
            if(j == tlen - 1)
            {
                ans ++;
                j = next[j];
            }
            else
                i ++, j ++;
        }
        else
            j = next[j];
    }
    return ans;
}
int main()
{
    int t;
    for(scanf("%d", &t); t --; )
    {
        scanf("%s%s", patStr, mainStr);
        BuildNext(patStr);
        printf("%d\n", KmpMatch(mainStr, patStr));
    }
    return 0;
}