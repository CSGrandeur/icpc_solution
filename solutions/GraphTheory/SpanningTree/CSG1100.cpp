// difficulty: 3
// Spanning Trees
// 知识点：基尔霍夫矩阵求最小生成树个数
// 题目的构图比较特别，其基尔霍夫矩阵是对角线加一些行列小于i的位置为-1的矩阵，
// 该矩阵全加到第一行会把第一行变成全1，再让有-1的行加上第一行，就变成上三角矩阵了。
// 在代码上，直接根据构思的对角矩阵连乘得解。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 1e5 + 10;
const int mod = 1e9 + 7;
int n;
char buf[maxn];
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        int ans = 1;
        scanf("%s", buf);
        for(int cnt = 1, i = strlen(buf) - 2; i > 0; i --)
        {
            ans = 1LL * ans * (buf[i] == '0' ? cnt : i + cnt + 1) % mod;
            if(buf[i] == '1') cnt ++;
        }
        printf("%d\n", ans);
    }
    return 0;
}