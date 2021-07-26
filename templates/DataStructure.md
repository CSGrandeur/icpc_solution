# 数据结构

## 哈希

### 数值哈希

按`unordered_map`接口设计

```cpp
const int hashmod = 1e4 + 3;
template<typename MPTP_K, typename MPTP_V>
struct HashTable
{
    struct HashNode
    {
        MPTP_K first;
        MPTP_V second;
        int nex;
    };
    std::vector<HashNode> ht;
    std::vector<int> rcd;
    HashTable(){rcd.resize(hashmod); Init();}
    typename std::vector<HashNode>::iterator begin(){return ht.begin();}
    typename std::vector<HashNode>::iterator end(){return ht.end();}
    void Init(){ht.clear(); std::fill(rcd.begin(), rcd.end(), -1);}
    void clear() {Init();}
    int size() {return ht.size();}
    int GetHash(MPTP_K x) {return (abs(x) ^ hashmod) % hashmod;}
    int Find(MPTP_K x)
    {
        for(int i = rcd[GetHash(x)]; i != -1; i = ht[i].nex)
            if(ht[i].first == x) return i;
        return -1;
    }
    int _Insert(MPTP_K x, MPTP_V v=0)
    {
        int hs = GetHash(x);
        HashNode tmp;
        tmp.nex = rcd[hs];
        tmp.first = x;
        tmp.second = v;
        ht.push_back(tmp);
        return rcd[hs] = ht.size() - 1;
    }
    HashNode &insert(MPTP_K x, MPTP_V v=0)
    {
        int ith = Find(x);
        return ht[ith == -1 ? _Insert(x, v) : ith];
    }
    bool count(MPTP_K x) {return Find(x) != -1;}
    MPTP_V &operator[](MPTP_K x){return insert(x).second;}
};
```

## 排序

### 归并排序

```cpp
int tmp[maxn];
MergeSort(int a[], int left, int right)
{
    if(left >= right - 1) return 0;
    int mid = left + right >> 1;
    MergeSort(a, left, mid);
    MergeSort(a, mid, right);
    int i, j, k;
    for(i = k = left, j = mid; i < mid && j < right;)
    {
        if(a[i] <= a[j]) tmp[k ++] = a[i ++];
        else tmp[k ++] = a[j ++];
    }
    while(i < mid) tmp[k ++] = a[i ++];
    while(j < right) tmp[k ++] = a[j ++];
    memcpy(a + left, tmp + left, sizeof(a[0]) * (right - left));
    return ret;
}
```
### 快速排序

```cpp
void QuickSort(int a[], int left, int right)
{
    if(left >= right - 1) return;
    int low = left, high = right - 1, center = a[low];
    while(low < high)
    {
        while(low < high && a[high] >= center) high --;
        a[low] = a[high];
        while(low < high && a[low] <= center) low ++;
        a[high] = a[low];
    }
    a[low] = center;
    QuickSort(a, left, low);
    QuickSort(a, low + 1, right);
}
```

## 二叉树

### 二叉树非递归遍历

层次遍历

```cpp
void LevelOrder()
{
    queue<int> q;
    q.push(0);
    while(!q.empty())
    {
        int now = q.front();
        q.pop();
        if(tr[now].v == '0')
            continue;
        printf("%c", tr[now].v);
        if(tr[now].l != -1) q.push(tr[now].l);
        if(tr[now].r != -1) q.push(tr[now].r);
    }
    printf("\n");
}
```

先序遍历

```cpp
void PreOrderNonRecursive()
{
    stack<int> stNode;
    int now = 0;
    if(tr[now].v == '#') return;  // '#' 表示空子树
    while(!stNode.empty() || tr[now].v != '#')
    {
        if(tr[now].v != '#')
        {
            printf("%c", tr[now].v);
            stNode.push(now);
            now = tr[now].l;
        }
        else if(!stNode.empty())
        {
            now = stNode.top();
            stNode.pop();
            now = tr[now].r;
        }
    }
    printf("\n");
}
```

中序遍历

```cpp
void InOrderNonRecursive()
{
    stack<int> stNode;
    int now = 0;
    if(tr[now].v == '#') return;
    while(!stNode.empty() || tr[now].v != '#')
    {
        if(tr[now].v != '#')
        {
            stNode.push(now);
            now = tr[now].l;
        }
        else if(!stNode.empty())
        {
            now = stNode.top();
            stNode.pop();
            printf("%c", tr[now].v);
            now = tr[now].r;
        }
    }
    printf("\n");
}
```

后序遍历

```cpp
void PostOrderNonRecursive()
{
    stack<int> stNode, stStatus;
    int now = 0;
    if(tr[now].v == '#') return;
    while (!stNode.empty() || now != -1 && tr[now].v != '#')
    {
        if(now == -1 || tr[now].v == '#')
        {  // 遇空指针（空树）出栈
            now = stNode.top();
            if(stStatus.top() == 0)  
            {  // 0 标记要往右去
                now = tr[now].r;
                stStatus.pop();
                stStatus.push(1);
            }
            else
            {  // 1 标记要返回（输出并出栈）
                printf("%c", tr[now].v);
                stNode.pop();
                stStatus.pop();
                now = -1;
            }
        }
        else
        {  // 非空则入栈，往左去
            stNode.push(now);
            stStatus.push(0);
            now = tr[now].l;
        }
    } 
    printf("\n");
}
```

### 哈夫曼编码

给定字符串求哈夫曼编码长度

```cpp
const int MAXCODE = 128;
int cnt[MAXCODE];
int GetHuffmanLength(char st[])
{
    int length = 0;
    priority_queue<int, vector<int>, greater<int> > q;
    memset(cnt, 0, sizeof(cnt));
    for(int i = 0; st[i]; i ++)
        cnt[st[i]] ++;
    for(int i = 0; i < MAXCODE; i ++)
        if(cnt[i]) q.push(cnt[i]);
    for(length = 0; q.size() > 1; )
    {
        int left = q.top(); q.pop();
        int right = q.top(); q.pop();
        length += left + right;
        q.push(left + right);
    }
    if(length == 0 && !q.empty())
        length = q.top();
    return length;
}
```

给定权值序列求赫夫曼树带权路径和

```cpp
LL GetHuffmanSum(int w[], int wLen)
{
    LL length = 0;
    priority_queue<LL, vector<LL>, greater<LL> > q;
    for(int i = 0; i < wLen; i ++)
        q.push(w[i]);
    for(length = 0; q.size() > 1; )
    {
        int left = q.top(); q.pop();
        int right = q.top(); q.pop();
        length += left + right;
        q.push(left + right);
    }
    return length;
}
```

## 平衡树

### AVL

```cpp
// 以POJ3481为例，插入 data 与 key，以 key 为排序依据
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
const int maxn = 1e5 + 10;
struct Node
{
    int key;
    int data;
    int left;
    int right;
    int height;
    void Init(){data = key = left = right = height = -1;}
    void Init(int k_, int d_=0){Init(); key = k_; data = d_;}
    Node(){Init();}
    Node(int k_, int d_=0){Init(k_, d_);}
};

Node tr[maxn];
int root, tp; // root 为可变的根，tp全局内存指针，都初始化为-1

inline int UpdateHeight(int now)
{
    int lh = tr[now].left == -1 ? 0 : tr[tr[now].left].height;
    int rh = tr[now].right == -1 ? 0 : tr[tr[now].right].height;
    return tr[now].height = (lh > rh ? lh : rh) + 1;
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
void Delete(int &now, int key)
{
    if(now == -1) return;
    else if(key < tr[now].key) Delete(tr[now].left, key);
    else if(key > tr[now].key) Delete(tr[now].right, key);
    else
    {
        if(tr[now].left == -1) now = tr[now].right;
        else if(tr[now].right == -1) now = tr[now].left;
        else
        {
            int nd;
            for(nd = tr[now].left; tr[nd].right != -1; nd = tr[nd].right);
            tr[now].key = tr[nd].key; tr[now].data = tr[nd].data;
            Delete(tr[now].left, tr[nd].key);
            if(HeightDiff(now) == 2)
                now = HeightDiff(tr[now].left) == 1 ? LL(now) : LR(now);
            else if(HeightDiff(now) == -2)
                now = HeightDiff(tr[now].right) == -1 ? RR(now) : RL(now);
            UpdateHeight(now);
        }
    }
}

int GetFirst(int now)
{
    if(now == -1) return now;
    for(; tr[now].left != -1; now = tr[now].left);
    return now;
}
int GetLast(int now)
{
    if(now == -1) return now;
    for(; tr[now].right != -1; now = tr[now].right);
    return now;
}

int main()
{
    int op, key, data;
    for(root = tp = -1; scanf("%d", &op) && op; )
    {
        if(op == 1)
        {
            scanf("%d%d", &data, &key);
            Insert(root, key, data);
        }
        else if(op == 2)
        {
            int ith = GetLast(root);
            printf("%d\n", ith == -1 ? 0 : tr[ith].data);
            if(ith != -1) Delete(root, tr[ith].key);
        }

        else if(op == 3)
        {
            int ith = GetFirst(root);
            printf("%d\n", ith == -1 ? 0 : tr[ith].data);
            if(ith != -1) Delete(root, tr[ith].key);
        }
        else
            break;
    }
    return 0;
}
```

## 树状数组

```cpp
// 前缀最大值为例
// 求后缀可将 Update 与 Search 的 x +- lowbit(x) 正负号反过来
int maxbi, di[maxn];
inline int lowbit(const int &x)
{
    return x & (-x);
}
void Update(int x, int v)
{
    for(; x <= maxbi; x += x & -x)
        di[x] = max(di[x], v);
}
int Search(int x)
{
    int res = 0;
    for(; x; x -= x & -x)
        res = max(res, di[x]);
    return res;
}
```

## 线段树

### 四叉树

```cpp
// 以求区域和为例
const int maxn = 1e5 + 10;
const int maxm = maxn << 4;
const int minco = -1e4 - 210;
const int maxco = 1e4 + 210;
int dr[4][maxm], val[maxm];
int tp;  // 模拟内存分配指针，初始化为 1 （0结点为根）
void InitNode(int qd, int now)
{
    if(dr[qd][now] == -1)
    {
        val[tp] = 0;
        dr[qd][now] = tp ++;
    }
}
void Update(int now, int v, int left, int right, int up, int down, int x, int y)
{
    // Update(0, v, minco, maxco, minco, maxco, x, y);
    // 格点x、y放入数值 v，约定不重复向同一位置放
    if(left == right || up == down)
        return;
    val[now] += v;
    if(left == right - 1 && up == down - 1)
        return;
    int lrmid = left + right >> 1;
    int udmid = up + down >> 1;
    if(x < lrmid && y < udmid) InitNode(0, now), Update(dr[0][now], v, left, lrmid, up, udmid, x, y);
    if(x < lrmid && y >= udmid) InitNode(1, now), Update(dr[1][now], v, left, lrmid, udmid, down, x, y);
    if(x >= lrmid && y < udmid) InitNode(2, now), Update(dr[2][now], v, lrmid, right, up, udmid, x, y);
    if(x >= lrmid && y >= udmid) InitNode(3, now), Update(dr[3][now], v, lrmid, right, udmid, down, x, y);
}
int Query(int now, int left, int right, int up, int down, int sl, int sr, int su, int sd)
{
    // Query(0, minco, maxco, minco, maxco, sl, sr, su, sd);
    // x∈[sl, sr)，y∈[su, sd) 的矩形范围个点数之和
    if(now == -1 || left == right || up == down)
        return 0;
    if(sl <= left && sr >= right && su <= up && sd >= down)
        return val[now];
    int lrmid = left + right >> 1;
    int udmid = up + down >> 1;
    int res = 0;
    if(sl < lrmid && su < udmid) res += Query(dr[0][now], left, lrmid, up, udmid, sl, sr, su, sd);
    if(sl < lrmid && sd >= udmid) res += Query(dr[1][now], left, lrmid, udmid, down, sl, sr, su, sd);
    if(sr >= lrmid && su < udmid) res += Query(dr[2][now], lrmid, right, up, udmid, sl, sr, su, sd);
    if(sr >= lrmid && sd >= udmid) res += Query(dr[3][now], lrmid, right, udmid, down, sl, sr, su, sd);
    return res;
}
```

### 常规线段树

线段树灵活性很强，重在理解和使用，无法固定模板，这里仅以“区间加等差数列lasy标记、统计区间和”为例。

```cpp
struct SegTree
{
    int seg[maxn << 5];
    int lasyFirst[maxn << 5], lasyDif[maxn << 5];
    SegTree(){Init();}
    // 初始化用实际输入尺寸 n 以免复杂度过高
    void Init(){memset(lasyFirst, -1, sizeof(int) * (n << 5)); memset(seg, 0, sizeof(int) * (n << 5));}
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
```

## 可持久化线段树

### 常规定义

```cpp
const int maxN = 1e6 + 10;      // 数组大小
const int maxM = maxN << 5;     // 线段树结点占用，基本上是 maxN * log2(max value)
const int maxV = 1e9 + 10;      // 离散数值的最大值
int rt[maxN], lc[maxM], rc[maxM];   // root、左“指针”、右“指针”
int tp;                         // 全局自增新建结点“指针”
```

### 区间数值和

```cpp
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
    // 通过传参，tl对应l-1前缀的版本，tr对应r前缀的版本，在树上求前缀差
    if(st <= left && ed >= right) return val[tr] - val[tl];
    int mid = left + right >> 1;
    LL res = 0;
    if(st < mid) res += Query(st, ed, left, mid, lc[tl], lc[tr]);
    if(ed > mid) res += Query(st, ed, mid, right, rc[tl], rc[tr]);
    return res;
}
```

## KD树

通用维度，动态内存，性能稍差

```cpp
#include<math.h>
#include<algorithm>
#include<vector>
using namespace std;
typedef int KD_CO;
typedef int KD_V;
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
    // #define Gvl(ar, p) (p >= 0 && p < ar.size() ? ar[p] : 0)
    inline KD_V Gvl(vector<int> &ar, int p) {return p >= 0 && p < ar.size() ? ar[p] : 0;}
    void BaseBuild();
    void InitIdx(int now){idx.clear(); _InitIdx(now);}
    void _InitIdx(int now);
    double _Variance(int l, int r, int dm);
    int Build(int l, int r);
    void CheckRebuild(int &now);
    void Maintain(int now);
    void Insert(KD_CO x_, KD_CO y_, KD_V v_=0);
    void Insert(vector<KD_CO> &co, KD_V v_=0);
    void _Insert(int &now, int last);
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
void KDTree::Insert(KD_CO x_, KD_CO y_, KD_V v_)
{
    vector<int> tmp = {x_, y_};
    _Add(tmp, v_);
    _Insert(root, v.size() - 1);
}
void KDTree::Insert(vector<KD_CO> &co, KD_V v_)
{
    _Add(co, v_);
    _Insert(root, v.size() - 1);
}
void KDTree::_Insert(int &now, int last)
{
    if(now == -1)
    {
        now = last;
        Maintain(now);
        return;
    }
    if(ax[last][dm[now]] <= ax[now][dm[now]]) _Insert(lc[now], last);
    else _Insert(rc[now], last);
    Maintain(now);
    CheckRebuild(now);
}
```

### KD树统计区域权值和

```cpp
struct KDTree
{
    // ...
    KD_V QuerySum(vector<KD_CO> &cs, vector<KD_CO> &ce){return _QuerySum(root, cs, ce);}
    KD_V _QuerySum(int now, vector<KD_CO> &cs, vector<KD_CO> &ce);
};

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
```

### KD树求最近点

```cpp
#include<queue>
#include<utility>
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
    // 求最近 k 个点
    int SearchK(vector<KD_CO> &p, int k, pqueue &q){return _SearchK(root, p, k, q);}
    int _SearchK(int now, vector<KD_CO> &p, int k, pqueue &q);
};
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
```

## 最远曼哈顿距离

任意维度个数

```cpp
#include<algorithm>
#include<vector>
#include<utility>
using namespace std;
typedef int COTYPE;
typedef pair<COTYPE, int> pii;
vector<vector<COTYPE> > p;
vector<vector<pii> > mrcd;
inline COTYPE GetVal(vector<COTYPE> &sp, int status)
{
    COTYPE tmp = 0;
    for(int k = 0; k < sp.size(); k ++, status >>= 1)
        tmp += (status & 1 ? 1 : -1) * sp[k];
    return tmp;
}
void ManDisPre(vector<vector<COTYPE> > &p)
{
    int m = p.empty() ? 0 : p[0].size();
    mrcd.resize(1 << m);
    for(int j = (1 << m) - 1; j >= 0; j --)
        mrcd[j].resize(n);
    for(int i = 0; i < n; i ++)
        for(int j = (1 << m) - 1; j >= 0; j --)
            mrcd[j][i] = pii(GetVal(p[i], j), i);
    for(int j = (1 << m) - 1; j >= 0; j --)
        sort(mrcd[j].begin(), mrcd[j].end());
}
COTYPE ManDis(int spid, vector<vector<COTYPE> > &p)
{
    int n = p.size(), m = p.empty() ? 0 : p[0].size();
    COTYPE ret = 0;
    for(int j = (1 << m) - 1; j >= 0; j --)
        ret = max(ret, GetVal(p[spid], j) + mrcd[j ^ (1 << m) - 1][n - 1].first);
    return ret;
}
```