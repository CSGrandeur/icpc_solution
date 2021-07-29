// difficulty: 3
// 巧克力王国
// KD-Tree 比较创新的一道题了，正常建树，对每个子树范围矩形判断四个点满足 ax+by<c 的个数，4或0直接返回，部分满足则继续递归。
// 可以全部读入一次建树。动态建树最后一组数据可能会TLE
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
int Cover(vector<KD_CO> &cl, vector<KD_CO> &ch, int a, int b, int c)
{
    int ret = 0;
    ret += 1LL * cl[0] * a + 1LL * cl[1] * b < c;
    ret += 1LL * cl[0] * a + 1LL * ch[1] * b < c;
    ret += 1LL * ch[0] * a + 1LL * cl[1] * b < c;
    ret += 1LL * ch[0] * a + 1LL * ch[1] * b < c;
    return ret;
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
    KD_V QuerySum(int a, int b, KD_V c){return _QuerySum(root, a, b, c);}
    KD_V _QuerySum(int now, int a, int b, KD_V c);
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
KD_V KDTree::_QuerySum(int now, int a, int b, KD_V c)
{
    if(now == -1) return 0;
    int cnt = Cover(cl[now], ch[now], a, b, c);
    if(cnt == 0) return 0;
    if(cnt == 4) return sum[now];
    int ret = 0;
    if(a * ax[now][0] + b * ax[now][1] < c) ret += v[now];
    return _QuerySum(Gid(lc, now), a, b, c) + _QuerySum(Gid(rc, now), a, b, c) + ret;
}
int main()
{
    int w, op, n, m, p, v, a, b;
    long long c;
    vector<int> s(2);
    KDTree kt;
    while(scanf("%d%d", &n, &m) != EOF)
    {
        kt.Init();
        for(int i = 0; i < n; i ++)
            scanf("%d%d%d", &s[0], &s[1], &v), kt._Add(s, v);
        kt.BaseBuild();
        for(int i = 0; i < m; i ++)
            scanf("%d%d%lld", &a, &b, &c), printf("%lld\n", kt.QuerySum(a, b, c));
    }
    return 0;
}