### `[CH弱省胡策R2]TATT`

> difficulty: 4

偏序问题，先按第一个维度排序可减少维度，将三个维度插入KD-Tree，用子树三个维度都更小的dp最大值做状态转移。

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
using namespace std;
const int maxn = 5e4 + 10;
typedef int KD_CO;
typedef int KD_V;
double Sqr(double x){return x * x;}
struct KDTree
{
    vector<vector<KD_CO> > ax, cl, ch;
    vector<KD_V> v, maxLen;
    vector<int> idx, lc, rc, dm, sz;
    const double alpha = 0.725;
    int root, dimN;
    void Init();
    bool _Add(vector<KD_CO> &co, KD_V v_=0);
    KDTree(){Init();}
    inline int Gid(vector<int> &ar, int p) {return p >= 0 && p < ar.size() ? ar[p] : -1;}
    // #define Gvl(ar, p) (p >= 0 && p < ar.size() ? ar[p] : 0)
    inline KD_V Gvl(vector<int> &ar, int p) {return p >= 0 && p < ar.size() ? ar[p] : 0;}
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
    void Query(int now, vector<int> &co, int &nowmax);
};
void KDTree::Init()
{
    root = dimN = -1;
    ax.clear();
    v.clear();
    idx.clear(), lc.clear(), rc.clear(), dm.clear();
    sz.clear(), maxLen.clear();
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
    sz.push_back(1), maxLen.push_back(v_);
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
    double maxLen = 0, mean, vmaxLen = 0;
    for(int i = l; i < r; i ++)
        maxLen += ax[idx[i]][dm];
    mean = maxLen / (r - l);
    for(int i = l; i < r; i ++)
        vmaxLen += Sqr(ax[idx[i]][dm] - mean);
    return vmaxLen / (r - l);
}
void KDTree::Maintain(int now)
{
    sz[now] = Gvl(sz, Gid(lc, now)) + Gvl(sz, Gid(rc, now)) + 1;
    maxLen[now] = max(max(Gvl(maxLen, Gid(lc, now)), Gvl(maxLen, Gid(rc, now))), v[now]);
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

bool JudgeCover(vector<int> &c, vector<int> &ax)
{
    for(int i = 0; i < c.size(); i ++)
        if(c[i] > ax[i]) return false;
    return true;
}
void KDTree::Query(int now, vector<int> &co, int &nowmax)
{
    if(now == -1) return;
    if(maxLen[now] < nowmax) return;
    if(JudgeCover(ch[now], co)) {nowmax = max(nowmax, maxLen[now]); return;}
    if(!JudgeCover(cl[now], co)) return ;
    if(JudgeCover(ax[now], co)) nowmax = max(nowmax, v[now]);
    Query(lc[now], co, nowmax);
    Query(rc[now], co, nowmax);
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
KDTree kt;
vector<vector<int> >co(maxn, vector<int>(4));

int main()
{
    int n;
    while(scanf("%d", &n) != EOF)
    {
        kt.Init();
        for(int i = 0; i < n; i ++)
        {
            for(int j = 0; j < 4; j ++)
                scanf("%d", &co[i][j]);
        }
        vector<vector<int> >::iterator it = co.begin();
        sort(it, it + n);
        int ans = 0;
        for(int i = 0; i < n; i ++)
        {
            vector<int> tmpco = {co[i][1], co[i][2], co[i][3]};
            int v = 0;
            kt.Query(kt.root, tmpco, v);
            v ++;
            kt.Insert(tmpco, v);
            ans = max(ans, v);
        }
        printf("%d\n", ans);
    }
    return 0;
}
```

