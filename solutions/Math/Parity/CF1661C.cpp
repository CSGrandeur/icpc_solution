// difficulty: 3
// Water the Trees
// 奇数天长1，偶数天长2，每天至多选一棵树，最少天数让所有树等高
// 出现1、2规则很有可能与奇偶性有关。可发现最优解只可能出现在maxHeight、和maxHeight+1这两种目标高度。
// 这两种目标高度奇偶性不同，根据奇偶性可确定至少需要的 1 的个数，因为满足奇偶性后，就可以随意分配了，
// 在此前提下计算“两天”的个数，每两天可提供3的高度。
// 总天数如果超出目标值 2 时，可减少一天（最后一个偶数天）。
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
const int maxn = 3e5 + 10;
typedef long long LL;
int n;
int t;
LL sum, mx, js, ans;
LL h[maxn];
LL Test(LL tg)
{
    LL need = tg * n - sum, bs1;
    if(tg & 1) bs1 = n - js;
    else bs1 = js;
    LL day = (need + 2) / 3;
    if(day < bs1) day = bs1;
    if(day * 3 - need >= 2)
        return day * 2 - 1;
    return day * 2;
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        sum = mx = js = 0;
        ans = 1e17;
        scanf("%d", &n);
        for(int i = 0; i < n; i ++)
            scanf("%lld", &h[i]), sum += h[i], mx = std::max(h[i], mx), js += h[i] & 1;
        for(LL i = mx; i < mx + 2; i ++)
            ans = std::min(ans, Test(i));
        printf("%lld\n", ans);    
    }
    return 0;
}