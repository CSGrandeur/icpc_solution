// difficulty: 3
// Absolute Difference Equation
// 很有高级感的一道题，经验与思维。
// 一个01串 a[i]，不断进行 a[i] = |a[i] - a[i+1]| 缩短串长度，直到剩下一个。串里有一些问号“?”，求问号分配0、1的方案总数，使最终剩下的数为 1 。
// 第一步，绝对值符号很恼人，本题最关键的一步就是 0、1 的相减绝对值相当于 异或 操作。这其实是个很古典的经验，多年没做给忘了。
// 变异或以后，就很容易推得最终表达式，每个 a[i] 贡献的次数，是从一个系数 1 开始不断移位后对位相加：
// 1
// 1 1
// 1 2 1
// 1 3 3 1
// 1 4 6 4 1
// ..
// 第二步突破点是经典的杨辉三角 ==> 组合数。
// 异或的特点是，偶数次异或无论 0 还是 1 结果都为 0 ，所以系数为偶数的确定值都可以不考虑。
// 系数为奇数的 0 不考虑。系数为奇数的 1 纳入考虑，如果有奇数个，则基底为 1，否则为 0 。
// 系数为偶数的 “?” 是 0 还是 1 都可以，设个数为 cnte ，则它们贡献的情况数是 2^cnte 。系数为奇数的 “?” 设有 cnto 个，用组合数累加得到。
// 基底是 1 的情况，则在 cnto 个“?”里选奇数个为 1，否则选偶数个为 1 ，乘上偶数个“?”贡献的 2^cnte 。
// 如果没有“?”，则基底为 1 就 1 种情况，为 0 就 0 种情况。
// 杨辉三角上的组合数系数计算，要判断奇偶，就不能同余取模了，其实可以不实质计算这些系数，就在组合数计算的流程中统计系数 2 的个数即可。

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
const int maxn = 1e6 + 10;
const int mod = 1e9 + 7;
char buf[maxn];
int n;
int invList[maxn], p2List[maxn];
void GetInvList(int mod)
{
    invList[1] = 1;
    for(int i = 2; i < maxn; i ++)
        invList[i] = 1LL * (mod - mod / i) * invList[mod % i] % mod;
}
void GetP2List(int mod)
{
    p2List[0] = 1;
    for(int i = 1; i < maxn; i ++)
        p2List[i] = (p2List[i - 1] << 1) % mod;
}
int Count2(int x)
{
    int cnt = 0;
    while(~x & 1)
        cnt ++, x >>= 1;
    return cnt;
}
int main()
{
    GetInvList(mod);
    GetP2List(mod);
    while(scanf("%s", buf) != EOF)
    {
        n = strlen(buf);
        int cm = 0, base = 0, cnto = 0, cnte = 0;
        for(int i = 0; i < n; i ++)
        {
            if(i) cm += Count2(n - i) - Count2(i);
            if(buf[i] == '0') continue;
            if(buf[i] == '1') base ^= !cm;
            else cnto += !cm, cnte += !!cm;
        }
        int ans = 0;
        cm = 1;
        for(int i = 0; i <= cnto; i ++)
        {
            if(i) cm = 1LL * cm * (cnto - i + 1) % mod * invList[i] % mod;
            if(i + base & 1) ans = (ans + 1LL * cm * p2List[cnte] % mod) % mod;
        }
        printf("%d\n", ans ? ans : base);
    }
    return 0;
}