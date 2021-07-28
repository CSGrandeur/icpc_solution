// difficulty: 3
// [Violet]天使玩偶/SJY摆棋子
// 已经练习过几道 KD-Tree 之后，这道题基本就是套模板了。
// 讨论区有不少说 KD-Tree 卡常，实测似乎不严重，开-O2最慢数据1.52秒
// 关键剪枝是点到另一个子树四边形的曼哈顿距离是否小于当前最优距离。点到四边形曼哈顿距离见 RecDis()
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<utility>
using namespace std;
// KD-Tree
typedef pair<int, int> pii;
typedef priority_queue<pii> pqueue;
typedef int KD_CO;
typedef int KD_V;
double Sqr(double x){return x * x;}
int Dis(vector<KD_CO> &a, vector<KD_CO> &b)
{
    int ret = 0;
    for(int i = 0; i < a.size(); i ++)
        ret += abs(a[i] - b[i]);
    return ret;
}
int RecDis(vector<KD_CO> &a, vector<KD_CO> &cl, vector<KD_CO> &ch)
{
    int ret = 0;
    for(int i = 0; i < a.size(); i ++)
        ret += a[i] < cl[i] ? cl[i] - a[i] : (a[i] < ch[i] ? 0 : a[i] - ch[i]);
    return ret;
}
typedef int KD_CO;
typedef int KD_V;
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
int KDTree::_SearchK(int now, vector<KD_CO> &p, int k, pqueue &q)
{
    if(now == -1) return 0;
    bool searchL = p[dm[now]] <= ax[now][dm[now]];
    if(searchL) _SearchK(Gid(lc, now), p, k, q);
    else _SearchK(Gid(rc, now), p, k, q);
    int dis = Dis(p, ax[now]);
    if(q.size() < k || q.top().first > dis)
    {
        q.push(pii(dis, now));
        while(q.size() > k) q.pop();
    }
    vector<int> &lrc = searchL ? rc : lc;
    if(q.size() < k || Gid(lrc, now) != -1 && q.top().first > RecDis(p, cl[lrc[now]], ch[lrc[now]]))
        _SearchK(Gid(lrc, now), p, k, q);
    return 0;
}
int n, m, op;
int main()
{
    vector<int> p(2);
    while(scanf("%d%d", &n, &m) != EOF)
    {
        KDTree kt;
        for(int i = 0; i < n; i ++)
        {
            scanf("%d%d", &p[0], &p[1]);
            kt._Add(p);
        }
        kt.BaseBuild();
        for(int i = 0; i < m; i ++)
        {
            scanf("%d%d%d", &op, &p[0], &p[1]);
            if(op == 1) {kt.Insert(p); }//if(kt.Insert(p) > sqrt(m)) kt.BaseBuild();}
            else
            {
                pqueue q;
                kt.SearchK(p, 1, q);
                printf("%d\n", q.top().first);
            }
        }
    }
    return 0;
}