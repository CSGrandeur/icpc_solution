### `[BOI2007]Mokia 摩基亚`

> difficulty: 3

luoguP4148 的弱化版，套模板。

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
using namespace std;
typedef int KD_CO;
typedef long long KD_V;
double Sqr(double x){return x * x;}
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
    #define Gvl(ar, p) (p >= 0 && p < ar.size() ? ar[p] : 0)
    // inline KD_V Gvl(vector<int> &ar, int p) {return p >= 0 && p < ar.size() ? ar[p] : 0;}
    void BaseBuild();
    void InitIdx(int now){idx.clear(); _InitIdx(now);}
    void _InitIdx(int now);
    double _Variance(int l, int r, int dm);
    int Build(int l, int r);
    void CheckRebuild(int &now);
    void Maintain(int now);
    int Insert(KD_CO x_, KD_CO y_, KD_V v_=0);
    int Insert(vector<KD_CO> &co, KD_V v_=0);
    int _Insert(int &now, int last);
    KD_V QuerySum(vector<KD_CO> &cs, vector<KD_CO> &ce){return _QuerySum(root, cs, ce);}
    KD_V _QuerySum(int now, vector<KD_CO> &cs, vector<KD_CO> &ce);
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
    lc.push_back(-1), rc.push_back(-1), dm.push_back(0);
    sz.push_back(1), sum.push_back(v_);
    cl.push_back(vector<int>(dimN));
    ch.push_back(vector<int>(dimN));
    return true;
}
void KDTree::BaseBuild()
{
    if(v.size() == 0) return;
    root = -1;
    idx.resize(v.size());
    for(int i = 0; i < idx.size(); i ++) idx[i] = i;
    Build(0, v.size());
}
void KDTree::_InitIdx(int now)
{
    if(now == -1) return;
    _InitIdx(Gid(lc, now));
    idx.push_back(now);
    _InitIdx(Gid(rc, now));
}
double KDTree::_Variance(int l, int r, int dm)
{
    if(l >= r) return 0;
    double sum = 0, mean, vsum = 0;
    for(int i = l; i < r; i ++)
        sum += ax[idx[i]][dm];
    mean = sum / (r - l);
    for(int i = l; i < r; i ++)
        vsum += Sqr(ax[idx[i]][dm] - mean);
    return vsum / (r - l);
}
void KDTree::Maintain(int now)
{
    sz[now] = Gvl(sz, Gid(lc, now)) + Gvl(sz, Gid(rc, now)) + 1;
    sum[now] = Gvl(sum, Gid(lc, now)) + Gvl(sum, Gid(rc, now)) + v[now];
    for(int i = 0; i < dimN; i ++)
        cl[now][i] = ch[now][i] = ax[now][i];
    if(now < lc.size() && lc[now] != -1)
        for(int i = 0; i < dimN; i ++)
        {
            cl[now][i] = min(cl[now][i], cl[lc[now]][i]);
            ch[now][i] = max(ch[now][i], ch[lc[now]][i]);
        }
    if(now < rc.size() && rc[now] != -1)
        for(int i = 0; i < dimN; i ++)
        {
            cl[now][i] = min(cl[now][i], cl[rc[now]][i]);
            ch[now][i] = max(ch[now][i], ch[rc[now]][i]);
        }
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
    Maintain(idx[mid]);
    return idx[mid];
}
void KDTree::CheckRebuild(int &now)
{
    if(alpha * sz[now] > max(Gvl(sz, Gid(lc, now)), Gvl(sz, Gid(rc, now))))
        return;
    InitIdx(now);
    now = Build(0, idx.size());
}
int KDTree::Insert(KD_CO x_, KD_CO y_, KD_V v_)
{
    vector<int> tmp = {x_, y_};
    _Add(tmp, v_);
    return _Insert(root, v.size() - 1);
}
int KDTree::Insert(vector<KD_CO> &co, KD_V v_)
{
    _Add(co, v_);
    return _Insert(root, v.size() - 1);
}
int KDTree::_Insert(int &now, int last)
{
    if(now == -1)
    {
        now = last;
        Maintain(now);
        return 0;
    }
    int depth;
    if(ax[last][dm[now]] <= ax[now][dm[now]]) depth = _Insert(lc[now], last);
    else depth = _Insert(rc[now], last);
    Maintain(now);
    CheckRebuild(now);
    return depth + 1;
}
KD_V KDTree::_QuerySum(int now, vector<KD_CO> &cs, vector<KD_CO> &ce)
{
    if(now == -1) return 0;
    int i;
    for(i = 0; i < dimN; i ++)
        if(cs[i] > ch[now][i] || ce[i] <= cl[now][i]) return 0;
    for(i = 0; i < dimN && cs[i] <= cl[now][i] && ce[i] > ch[now][i]; i ++);
    if(i == dimN) return sum[now];
    KD_V ret = 0;
    for(i = 0; i < dimN && cs[i] <= ax[now][i] && ce[i] > ax[now][i]; i ++);
    if(i == dimN) ret += v[now];
    return _QuerySum(Gid(lc, now), cs, ce) + _QuerySum(Gid(rc, now), cs, ce) + ret;
}
int main()
{
    int w, op, n, m, p, v;
    vector<int> s(2), e(2);
    KDTree kt;
    while(scanf("%d%d", &op, &w) != EOF)
    {
        kt.Init();
        while(scanf("%d", &op) && op != 3)
        {
            if(op == 1)
            {
                scanf("%d%d%d", &s[0], &s[1], &v);
                kt.Insert(s, v);
            }
            else
            {
                scanf("%d%d%d%d", &s[0], &s[1], &e[0], &e[1]);
                e[0] ++;
                e[1] ++;
                printf("%lld\n", kt.QuerySum(s, e));
            }
        }
    }
    return 0;
}
```

