// difficulty: 4
// Stone Games
// 该题难点在证明：
// `[1,x]` 的数都能被组合得到时，`[1,x+1]`范围的数的和`sum`，`[1,sum]`的数一定也都能被求和得到。
// 证明：
// 1. 只要`a`能够组合得到，`[1,a-x]`就能组合得到
// 2. `(x, sum]`之间，有一些数`{a1, a2, ..., ak | ai < aj && aj - ai <= x + 1}` 必然可以得到，
// 即这些数的跨度不会大于`x+1`，因为组成它们的数都在`[1,x+1]`范围内。
// 结合 `1` 与 `2`，`(x+1, sum]` 之间的数都能够表示为 `a-x`，即都能够得到。
// 通过迭代`[1, x+1]` 的数的和，到 `sum == x` 时终止，此时 `sum+1` 即最小的无法组合得到的数
// 迭代增长很快，可以理解为 `logn`
// 问题建模为求数组`[left, right)`区间上，数值在`[1, x]`范围的和，可持久化线段树实现

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<vector>
#include<algorithm>
typedef long long LL;
const int maxN = 1e6 + 10;
const int maxM = maxN << 5;
const int maxV = 1e9 + 10;
int rt[maxN], lc[maxM], rc[maxM];
LL val[maxM];
int s[maxN];
int tp;
int n, q, l, r;
void Update(int v, int left, int right, int &now, int last)
{
    // 沿着last版本的树，更新now版本的一条链
    now = tp ++;
    lc[now] = lc[last];
    rc[now] = rc[last];
    val[now] = val[last] + v;   // now版本下[left, right) 范围内所有数的和
    if(left >= right - 1) return;
    int mid = left + right >> 1;
    if(v < mid) Update(v, left, mid, lc[now], lc[last]);
    else Update(v, mid, right, rc[now], rc[last]);
}
LL Query(int st, int ed, int left, int right, int tl, int tr)
{
    // 求编号区间[l,r)的数值在[st, ed)范围的数的和
    // 通过传参，tl对应1~l-1区间的版本，tr对应1~r区间的版本，在树上求前缀差
    if(st <= left && ed >= right) return val[tr] - val[tl];
    int mid = left + right >> 1;
    LL res = 0;
    if(st < mid) res += Query(st, ed, left, mid, lc[tl], lc[tr]);
    if(ed > mid) res += Query(st, ed, mid, right, rc[tl], rc[tr]);
    return res;
}
int main()
{
    while(scanf("%d%d", &n, &q) != EOF)
    {
        tp = 1;
        for(int i = 1; i <= n; i ++)
        {
            scanf("%d", &s[i]);
            Update(s[i], 1, maxV, rt[i], rt[i - 1]);
        }
        LL ans = 0, gainSum;
        while(q --)
        {
            scanf("%d%d", &l, &r);
            l = (l + ans) % n + 1;
            r = (r + ans) % n + 1;
            if(l > r) std::swap(l, r);
            for(ans = 0; ; ans = gainSum)
            {
                gainSum = Query(1, std::min(ans + 2, (LL)maxV), 1, maxV, rt[l - 1], rt[r]);
                if(gainSum == ans) break;
            }
            ans ++;
            printf("%lld\n", ans);
        }
    }
    return 0;
}