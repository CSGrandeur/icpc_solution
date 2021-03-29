// 还是KMP统计个数，只是输入别致一些
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
char mainStr[1000], patStr[1000];
int next[1000];
void BuildNext(char s[])
{
    next[0] = -1;
    for(int i = 0, j = -1; s[i]; )
    {
        if(j == -1 || s[i] == s[j])
            next[++i] = ++j;
        else
            j = next[j];
    }
}
int KmpMatch(char ts[])
{
    int tlen = strlen(ts);
    int ans = 0, j = -1;
    char c = getchar();
    while(c != '#')
    {
        c = tolower(c);
        if(j == -1 || c == ts[j])
        {
            if(j == tlen - 1)
            {
                ans ++;
                c = getchar();
                j = next[j];
            }
            else
            {
                c = getchar();
                j ++;
            }
        }
        else
            j = next[j];
    }
    return ans;
}

int main()
{
    strcpy(patStr, "doge");
    BuildNext(patStr);
    printf("%d\n", KmpMatch(patStr));
    return 0;
}