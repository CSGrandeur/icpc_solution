// difficulty: 3
// Mobile Computing
// 枚举二叉树结构。
// 方式1 利用二进制枚举集合的经典练习
// 方式2 类似赫夫曼树的自底向上构建，代码没剪枝太暴力不推荐。
// 数据有坑：只有 1 个的时候输出 0 而不是无解

// 方式1：二进制枚举，存子树的伸展距离
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<algorithm>
#include<vector>
const int maxn = 6;
const int maxm = 1 << maxn | 1;
int t, s, tp, w[maxn], sum[maxm], root;
bool vis[maxm];
double r, ans;
std::vector<double> lre[maxm], rre[maxm];

void DFS(int ndSet)
{
    if(vis[ndSet]) return;
    lre[ndSet].clear();
    rre[ndSet].clear();
    vis[ndSet] = true;
    if(!(ndSet - 1 & ndSet))
    {
        lre[ndSet].push_back(0), rre[ndSet].push_back(0);
        if(ndSet == root)  ans = std::max(ans, 0.0);
        return;
    }
    for(int left = ndSet - 1 & ndSet; left; left = left - 1 & ndSet)
    {
        int right = ndSet ^ left;
        DFS(left), DFS(right);
        double lreach = -(double)sum[right] / sum[ndSet];
        double rreach = (double)sum[left] / sum[ndSet];
        for(int i = 0; i < lre[left].size(); i ++)
        {
            for(int j = 0; j < lre[right].size(); j ++)
            {
                double ltmp = std::min(lreach + lre[left][i], rreach + lre[right][j]);
                double rtmp = std::max(lreach + rre[left][i], rreach + rre[right][j]);
                if(rtmp - ltmp < r) 
                {
                    lre[ndSet].push_back(ltmp), rre[ndSet].push_back(rtmp);
                    if(ndSet == root) ans = std::max(ans, rtmp - ltmp);
                }
            }
        }
    }   
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        scanf("%lf%d", &r, &s);
        memset(vis, 0, sizeof(vis));
        memset(sum, 0, sizeof(sum));
        root = (1 << s) - 1;
        for(int i = 0; i < s; i ++)
        {
            scanf("%d", &w[i]);
            for(int j = root; j; j --)
            {
                if(j >> i & 1)
                    sum[j] += w[i];
            }
        }
        ans = -1;
        DFS(root);
        printf("%.10f\n", ans);
    }
    return 0;
}

// 方式2： 自底向上枚举。没想到好的剪枝方式，非常暴力非常不优雅
/*
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<algorithm>
#include<set>
struct Node
{
    int w;
    int l;
    int r;
    void Init() {w = l = r = -1;}
};
Node tr[100];
int ndPool[100];
int t, s, w[10];
double r, ans;
double lmost, rmost;
void GetReach(int now, double lreach, double rreach)
{
    if(tr[now].l != -1)
    {
        int lw = tr[tr[now].l].w;
        int rw = tr[tr[now].r].w;
        double nlreach = lreach - (double)rw / (lw + rw);
        double nrreach = rreach + (double)(lw) / (lw + rw);
        GetReach(tr[now].l, nlreach, nlreach);
        GetReach(tr[now].r, nrreach, nrreach);
    }
    else
    {
        lmost = std::min(lreach, lmost);
        rmost = std::max(rreach, rmost);
    }
}
void DFS(std::set<int> se, int tp)
{
    if(se.size() == 1)
    {
        lmost = rmost = 0;
        GetReach(tp - 1, 0, 0);
        if(rmost - lmost < r)
            ans = std::max(ans, rmost - lmost);
        
        return;
    }
    for(auto it : se)
        for(auto jt : se)
        {
            if(it == jt)
                continue;
            std::set<int> setmp;
            for(auto kt : se)
            {
                if(kt != it && kt != jt)
                    setmp.insert(kt);
            }
            tr[tp].w = tr[it].w + tr[jt].w;
            tr[tp].l = it, tr[tp].r = jt;
            setmp.insert(tp);
            DFS(setmp, tp + 1);
        }
}
int main()
{
    for(scanf("%d", &t); t --; )
    {
        scanf("%lf%d", &r, &s);
        std::set<int> se;
        ans = -1;
        for(int i = 0; i < s; i ++)
        {
            tr[i].Init();
            scanf("%d", &tr[i].w);
            se.insert(i);
        }
        DFS(se, s);
        if(ans < 0)
            printf("-1\n");
        else
            printf("%.10f\n", ans);
    }
    return 0;
}
*/