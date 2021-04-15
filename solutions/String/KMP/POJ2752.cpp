// difficulty: 1
// Seek the Name, Seek the Fame
// 迭代输出前缀的nex，KMP的典型性质

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 4e5 + 10;
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
void DFS(int j)
{
    if(j <= 0) return;
    DFS(nex[j]);
    printf(nex[j] <= 0 ? "%d" : " %d", j);
}
int main()
{
    while(scanf("%s", s) != EOF)
    {
        BuildNext(s, nex);
        DFS(strlen(s)); // 递归逆序输出，懒得开数组存
        printf("\n");
    }
    return 0;
}