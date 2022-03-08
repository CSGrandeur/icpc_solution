// difficulty: 4
// Make Rounddog Very Happy
// 求以每个位置的数为止的前缀的，后缀最大值大于后缀长度的后缀个数。
// 数据范围显然不能 O(n^2)，通常思路是考虑每个数对答案的贡献，那么需要能够 logn 计算贡献。
// 每个数能够产生贡献的前提是，它是某个后缀里的最大值，那么需要计算这个数的"影响力范围"
// 求最大的区间，在这个区间内一个数是最大值，可以用单调栈。入栈时更新左端点，出栈时更新右端点。
// 对每个数以及它的影响力范围，能够对一个区间贡献等差数列，一个区间贡献相等的值（相等的值是特殊的等差数列）。
// 区间增加等差数列可以用线段树实现。
// 由于本题不需要动态查询，所以统计区间等差数列可以用效率更高的静态方法：二阶差分。
// 线段树版代码：
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 1e6 + 10;
int n, m;
int a[maxn];
int stk[maxn], stp;
int aleft[maxn], aright[maxn];
struct SegTree
{
    int seg[maxn << 3];
    int lasyFirst[maxn << 3], lasyDif[maxn << 3];
    SegTree(){Init();}
    void Init(){memset(lasyFirst, -1, sizeof(int) * (n << 3)); memset(seg, 0, sizeof(int) * (n << 3));}
    int BuildTree(int now, int left, int right, int *arr=NULL)
    {
        if(left >= right) return 0;
        int &sn = seg[now];
        if(left == right - 1) return sn = arr == NULL ? 0 : arr[left];
        int mid = left + right >> 1;
        if(mid > left) sn += BuildTree(now << 1, left, mid, arr);
        if(right > mid) sn += BuildTree(now << 1 | 1, mid, right, arr);
        return sn;
    }
    void UpdateNowNode(int now, int left, int right, int fst, int dif)
    {
        int rangeN = right - left;
        seg[now] += fst * rangeN + ((rangeN - 1) * rangeN * dif >> 1);
        if(lasyFirst[now] == -1) lasyFirst[now] = lasyDif[now] = 0;
        lasyFirst[now] += fst;
        lasyDif[now] += dif;
    }
    void PushDown(int now, int left, int right)
    {
        if(lasyFirst[now] == -1 || left >= right) return;
        int mid = left + right >> 1;
        UpdateNowNode(now << 1, left, mid, lasyFirst[now], lasyDif[now]);
        UpdateNowNode(now << 1 | 1, mid, right, lasyFirst[now] + (mid - left) * lasyDif[now], lasyDif[now]);
        lasyFirst[now] = lasyDif[now] = -1;
    }
    void PushUp(int now)
    {
        while(now)
        {
            seg[now] = seg[now << 1] + seg[now << 1 | 1];
            now >>= 1;
        }
    }
    int Query(int now, int left, int right, int i, int j)
    {
        if(left >= right) return 0;
        if(i <= left && j >= right) return seg[now];
        PushDown(now, left, right);
        int mid = left + right >> 1;
        int ret = 0;
        if(i < mid) ret += Query(now << 1, left, mid, i, j);
        if(j > mid) ret += Query(now << 1 | 1, mid, right, i, j);
        return ret;
    }
    void Add(int now, int left, int right, int i, int j, int fst, int dif)
    {
        if(left >= right) return;
        if(i <= left && j >= right)
            UpdateNowNode(now, left, right, fst + dif * (left - i), dif);
        else
        {
            PushDown(now, left, right);
            int mid = left + right >> 1;
            if(i < mid) Add(now << 1, left, mid, i, j, fst, dif);
            if(j > mid) Add(now << 1 | 1, mid, right, i, j, fst, dif);
            PushUp(now);
        }
    }
};
SegTree sgt;

void AddInfluence(int ith)
{
    if(a[ith] <= 0) return;
    int left = std::max(aleft[ith], ith - a[ith] + 1);
    int right = std::min(aright[ith], ith + a[ith] - 1);
    int leftFirst = ith - left + 1;
    sgt.Add(1, 0, n, ith, std::min(left + a[ith], right + 1), leftFirst, 0);
    if(left + a[ith] <= right)
        sgt.Add(1, 0, n, left + a[ith], right + 1, leftFirst - 1, -1);
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        stp = -1;
        sgt.Init();
        sgt.BuildTree(1, 0, 0, NULL);
        for(int i = 0; i < n; i ++)
        {
            scanf("%d", &a[i]);
            a[i] -= m;
            while(stp >= 0 && a[i] > a[stk[stp]])
                aright[stk[stp --]] = i - 1;
            aleft[i] = stp < 0 ? 0 : stk[stp] + 1;
            stk[++ stp] = i;
        }
        while(stp >= 0) aright[stk[stp --]] = n - 1;
        for(int i = 0; i < n; i ++)
            AddInfluence(i);
        for(int i = 0; i < n; i ++)
            printf(" %d" + !i, sgt.Query(1, 0, n, i, i + 1));
        printf("\n");
    }
    return 0;
}