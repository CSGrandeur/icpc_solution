// difficulty: 3
// The Closest M Points
// KD树找 k 近点，优先队列维护最近 k 个点，“当前”最近点个数不足 k 或最远的一个点距离大于分裂坐标分裂点距离则遍历另一侧子树。
// 优先队列为大值优先，这样可以快速找到k最近点中的最远点，并通过出队维护 k 近点。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<utility>
using namespace std;
typedef pair<double, int> pii;
typedef priority_queue<pii> pqueue;
typedef int KD_CO;
typedef int KD_V;
double Sqr(double x){return x * x;}
double Dis(vector<KD_CO> &a, vector<KD_CO> &b)
{
    double res = 0;
    for(int i = 0; i < a.size(); i ++)
        res += Sqr(a[i] - b[i]);
    return sqrt(res);    
}
struct KDTree
{
    vector<vector<KD_CO> > ax, cl, ch;
    vector<KD_V> v, sum;
    vector<int> idx, lc, rc, dm, sz;
    const double alpha = 0.725;
    int root, dimN;
    void Init();
    bool _Add(vector<KD_CO> &co, KD_V v_=0);
    KDTree(){Init();}
    inline int Gid(vector<int> &ar, int p) {return p >= 0 && p < ar.size() ? ar[p] : -1;}
    void BaseBuild();
    int Build(int l, int r);
    double _Variance(int l, int r, int dm);
    int SearchK(vector<KD_CO> &p, int k, pqueue &q){return _SearchK(root, p, k, q);}
    int _SearchK(int now, vector<KD_CO> &p, int k, pqueue &q);
};
void KDTree::Init()
{
    root = dimN = -1;
    ax.clear();
    v.clear();
    idx.clear(), lc.clear(), rc.clear(), dm.clear();
    sz.clear(), sum.clear();
    cl.clear();
    ch.clear();
}
bool KDTree::_Add(std::vector<KD_CO> &co, KD_V v_)
{
    if(dimN == -1) dimN = co.size();
    if(co.size() != dimN) return false;
    ax.push_back(co);
    v.push_back(v_);
    idx.push_back(-1), lc.push_back(-1), rc.push_back(-1), dm.push_back(0);
    sz.push_back(1), sum.push_back(v_);
    cl.push_back(vector<int>(dimN));
    ch.push_back(vector<int>(dimN));
    return true;
}
void KDTree::BaseBuild()
{
    if(v.size() == 0) return;
    root = -1;
    for(int i = 0; i < idx.size(); i ++) idx[i] = i; 
    Build(0, v.size());
}
double KDTree::_Variance(int l, int r, int dm)
{
    if(l >= r) return 0;
    double sum = 0, mean, vsum = 0;
    for(int i = l; i < r; i ++)
        sum += ax[dm][idx[i]];
    mean = sum / (r - l);
    for(int i = l; i < r; i ++)
        vsum += Sqr(ax[dm][idx[i]] - mean);
    return vsum / (r - l);
}
int KDTree::Build(int l, int r)
{
    if(l >= r) return -1;
    int mid = l + r >> 1;
    int vMaxAx = 0;
    double vMaxAxVar = 0;
    for(int i = 0; i < dimN; i ++)
    {
        double tmp = _Variance(l, r, i);
        if(tmp > vMaxAxVar)
            vMaxAx = i, vMaxAxVar = tmp;
    }
    vector<int>::iterator it = idx.begin();
    nth_element(it + l, it + mid, it + r, [&](int a, int b){return ax[a][vMaxAx] < ax[b][vMaxAx];});
    if(root == -1) root = idx[mid];
    dm[idx[mid]] = vMaxAx;
    lc[idx[mid]] = Build(l, mid);
    rc[idx[mid]] = Build(mid + 1, r);
    return idx[mid];
}
int KDTree::_SearchK(int now, vector<KD_CO> &p, int k, pqueue &q)
{
    if(now == -1) return 0;
    bool searchL = p[dm[now]] <= ax[now][dm[now]];
    if(searchL) _SearchK(Gid(lc, now), p, k, q);
    else _SearchK(Gid(rc, now), p, k, q);
    double dis = Dis(p, ax[now]);
    if(q.size() < k || q.top().first > dis)
    {
        q.push(pii(dis, now));
        while(q.size() > k) q.pop();
    }
    if(q.size() < k || q.top().first > abs(ax[now][dm[now]] - p[dm[now]]))
    {
        if(searchL) _SearchK(Gid(rc, now), p, k, q);
        else _SearchK(Gid(lc, now), p, k, q);
    }
    return 0;
}

int n, k, t, m;
int main()
{
    vector<int> p;
    KDTree kt;
    while(scanf("%d%d", &n, &k) != EOF)
    {
        kt.Init();
        p.resize(k);
        for(int i = 0; i < n; i ++)
        {
            for(int j = 0; j < k; j ++)
                scanf("%d", &p[j]);
            kt._Add(p);
        }
        kt.BaseBuild();
        scanf("%d", &t);
        while(t --)
        {
            for(int i = 0; i < k; i ++)
                scanf("%d", &p[i]);
            scanf("%d", &m);
            pqueue q;
            kt.SearchK(p, m, q);
            printf("the closest %d points are:\n", m);
            vector<vector<int> > ans;
            while(!q.empty())
                ans.push_back(kt.ax[q.top().second]), q.pop();
            for(int i = ans.size() - 1, o = 0; o < m; i --, o ++, printf("\n"))
                for(int j = 0; j < k; j ++)
                    printf(" %d" + !j, ans[i][j]);
        }
    }
    return 0;
}