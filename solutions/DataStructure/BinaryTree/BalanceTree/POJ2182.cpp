// difficulty: 3
// Lost Cows
// 从后往前找第 k 小
// 这个代码用平衡树实现
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e4 + 10;

struct Node
{
    int key;
    int data;
    int left;
    int right;
    int height;
    int sum;
    void Init(){data = key = left = right = height = sum = -1;}
    void Init(int k_, int d_=0){Init(); key = k_; data = d_;}
    Node(){Init();}
    Node(int k_, int d_=0){Init(k_, d_);}
};

Node tr[maxn];
int root, tp;
inline void UpdateHeight(int now)
{
    int lh = tr[now].left == -1 ? 0 : tr[tr[now].left].height;
    int rh = tr[now].right == -1 ? 0 : tr[tr[now].right].height;
    int ls = tr[now].left == -1 ? 0 : tr[tr[now].left].sum;
    int rs = tr[now].right == -1 ? 0 : tr[tr[now].right].sum;
    tr[now].height = (lh > rh ? lh : rh) + 1;
    tr[now].sum = ls + rs + 1;
}
inline int HeightDiff(int now)
{
    int lh = tr[now].left == -1 ? 0 : tr[tr[now].left].height;
    int rh = tr[now].right == -1 ? 0 : tr[tr[now].right].height;
    return lh - rh;
}
int LL(int an)
{
    int bn = tr[an].left;
    int dn = tr[bn].right;
    tr[bn].right = an;
    tr[an].left = dn;
    UpdateHeight(an);
    UpdateHeight(bn);
    return bn;
}
int RR(int an)
{
    int bn = tr[an].right;
    int dn = tr[bn].left;
    tr[bn].left = an;
    tr[an].right = dn;
    UpdateHeight(an);
    UpdateHeight(bn);
    return bn;
}
int LR(int an)
{
    tr[an].left = RR(tr[an].left);
    return LL(an);
}
int RL(int an)
{
    tr[an].right = LL(tr[an].right);
    return RR(an);
}
void Insert(int &now, int key, int data=0)
{
    if(now == -1)
    {
        now = ++ tp;
        tr[now].Init(key, data);
    }
    else if(key < tr[now].key)
    {
        Insert(tr[now].left, key, data);
        if(HeightDiff(now) == 2)
            now = key < tr[tr[now].left].key ? LL(now) : LR(now);
    }
    else if(key > tr[now].key)
    {
        Insert(tr[now].right, key, data);
        if(HeightDiff(now) == -2)
            now = key > tr[tr[now].right].key ? RR(now) : RL(now);
    }
    UpdateHeight(now);
}
int Delete(int &now, int key)
{
    int res = 0;
    if(now == -1) return 0;
    else if(key < tr[now].key) res = Delete(tr[now].left, key);
    else if(key > tr[now].key) res = Delete(tr[now].right, key);
    else
    {
        res = tr[now].data;
        if(tr[now].left == -1) now = tr[now].right;
        else if(tr[now].right == -1) now = tr[now].left;
        else
        {
            int nd;
            for(nd = tr[now].left; tr[nd].right != -1; nd = tr[nd].right);
            tr[now].key = tr[nd].key; tr[now].data = tr[nd].data;
            Delete(tr[now].left, tr[nd].key);
        }
    }
    if(now == -1) return res;
    UpdateHeight(now);
    if(HeightDiff(now) == 2)
        now = HeightDiff(tr[now].left) >= 0 ? LL(now) : LR(now);
    else if(HeightDiff(now) == -2)
        now = HeightDiff(tr[now].right) <= 0 ? RR(now) : RL(now);
    return res;
}

int Search(int now, int k)
{
    if(now == -1) return now;
    if(tr[now].left != -1 && tr[tr[now].left].sum >= k)
        return Search(tr[now].left, k);
    int kcut = k - (tr[now].left == -1 ? 0 : tr[tr[now].left].sum);
    if(kcut == 1) return now;
    return Search(tr[now].right, kcut - 1);
}

int n;
int a[maxn];
int main()
{
    while(scanf("%d", &n) != EOF)
    {
        a[0] = 0;
        root = tp = -1;
        for(int i = 1; i <= n; i ++)
            Insert(root, i);
        for(int i = 1; i < n; i ++)
            scanf("%d", &a[i]);
        for(int i = n - 1; i >= 0; i --)
        {
            int res = Search(root, a[i] + 1);
            a[i] = tr[res].key;
            Delete(root, tr[res].key);
        }
        for(int i = 0; i < n; i ++) printf("%d\n", a[i]);
    }
    return 0;
}