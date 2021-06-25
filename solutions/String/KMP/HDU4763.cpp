// difficulty: 2
// Theme Section
// 求最长的、不重叠的、相等的  前缀&后缀&中间子串，如果存在，则其一定在字符串结尾'\0'位置的next链里，也在中间串末尾的next链里
// 枚举中间串末尾与字符串末尾的next链，找两个next链相同的值，并且在这个值保证三个部分不重叠的情况下，更新答案（取最大值）
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
char buf[1100000];
int next[1100000];
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
int main()
{
    int t, ans;
    for(scanf("%d", &t); t --; )
    {
        scanf("%s", buf);
        BuildNext(buf);
        ans = 0;
        int buflen = strlen(buf);
        for(int i = 0; buf[i]; i ++)
            for(int j = next[i]; j != -1; j = next[j])
                for(int k = next[buflen]; k != -1; k = next[k])
                {
                    if(k == j && buflen - k >= i && i - j >= j)
                        ans = std::max(ans, j);
                }
        printf("%d\n", ans);
    }
    return 0;
}