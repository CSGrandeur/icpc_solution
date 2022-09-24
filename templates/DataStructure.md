# 数据结构

## 常用预处理

### 离散化

```cpp
template<typename TP_V>
struct Discretization
{
    std::vector<TP_V> b;
    std::unordered_map<TP_V, int> mp;
    void Init(){b.clear(); mp.clear();}
    Discretization(){Init();}
    Discretization(std::vector<TP_V> &a){Make(a);}
    void Make(std::vector<TP_V> &a)
    {
        Init();
        Add(a);
    }
    void Add(std::vector<TP_V> &a)
    {
        for(auto i : a) b.push_back(i);
        std::sort(b.begin(), b.end());
        b.erase(std::unique(b.begin(), b.end()), b.end());
        for(int i = 0; i < b.size(); i ++) mp[b[i]] = i;
    }
    unsigned size()
    {
        return b.size();
    }
    TP_V &operator[](int ith){return b[ith];}
    int Loc(TP_V x) {return mp.count(x) ? mp[x] : -1;}
};
```

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
    // x\in [sl, sr)，y\in [su, sd) 的矩形范围个点数之和
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

线段树灵活性很强，重在理解和使用，无法固定模板，这里仅以 区间加等差数列lasy标记、统计区间和 为例。

#### 预分配内存线段树

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

#### 动态开点线段树

以区间连续颜色种类数为例

```cpp
struct SegNode
{
    int lp, rp, l, r, x, lc, rc, lz;
    void Init(){x = lz = 0, lc = rc = -1, lp = rp = -1;}
    void Init(int l_, int r_){Init(); l = l_, r = r_;}
    SegNode(){Init();}
    SegNode(int l_, int r_){Init(l_, r_);}
};
std::vector<SegNode> sn;
struct SegTree
{
    int rt, segN;
    void Init(int n)
    {
        segN = n + 10;
        rt = sn.size();
        sn.push_back(SegNode(0, segN));
    }
    void CheckNodeP(int &tp, int l, int r)
    {
        if(tp != -1) return;
        tp = sn.size();
        sn.push_back(SegNode(l, r));
    }
    void Build(int now, std::vector<int> &vw)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l == sn[now].r - 1)
        {
            if(vw.size() > sn[now].l)
                sn[now].x = 1, sn[now].lc = sn[now].rc = vw[sn[now].l];
            return;
        }
        int mid = sn[now].l + sn[now].r >> 1;
        if(sn[now].l < mid)
        {
            CheckNodeP(sn[now].lp, sn[now].l, mid);
            Build(sn[now].lp, vw);
        }
        if(sn[now].r > mid)
        {
            CheckNodeP(sn[now].rp, mid, sn[now].r);
            Build(sn[now].rp, vw);
        }
        PushUp(now);
    }
    void SetNode(int now, int lz)
    {
        sn[now].x = 1;
        sn[now].lc = sn[now].rc = sn[now].lz = lz;
    }
    void PushDown(int now)
    {
        if(sn[now].l == sn[now].r - 1 || !sn[now].lz) return;
        if(sn[now].lp != -1)
            SetNode(sn[now].lp, sn[now].lz);
        if(sn[now].rp != -1)
            SetNode(sn[now].rp, sn[now].lz);
        sn[now].lz = 0;
    }
    void PushUp(int now)
    {
        sn[now].x = 0;
        if(sn[now].lp != -1) 
        {
            sn[now].x += sn[sn[now].lp].x;
            sn[now].lc = sn[sn[now].lp].lc;
        }
        if(sn[now].rp != -1)
        {
            sn[now].x += sn[sn[now].rp].x;
            sn[now].rc = sn[sn[now].rp].rc;
        }
        if(sn[now].lp != -1 && sn[now].rp != -1 && sn[sn[now].lp].rc == sn[sn[now].rp].lc)
            sn[now].x --;
        if(sn[now].x < 0) sn[now].x = 0;
    }
    void UpdateRange(int now, int l, int r, int k)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l >= l && sn[now].r <= r)
        {
            SetNode(now, k);
            return;
        }
        PushDown(now);
        int mid = sn[now].l + sn[now].r >> 1;
        if(l < mid && sn[now].lp != -1) UpdateRange(sn[now].lp, l, r, k);
        if(r > mid && sn[now].rp != -1) UpdateRange(sn[now].rp, l, r, k);
        PushUp(now);
    }
    SegNode SegMerge(const SegNode &ls, const SegNode &rs)
    {
        if(ls.lc == -1) return rs;
        if(rs.lc == -1) return ls;
        SegNode s(-1, -1);
        s.x = rs.x + ls.x - (rs.lc == ls.rc);
        s.lc = ls.lc;
        s.rc = rs.rc;
        return s;
    }
    SegNode Query(int now, int l, int r)
    {
        if(now == -2) now = this->rt;
        if(sn[now].l >= l && sn[now].r <= r) return sn[now];
        PushDown(now);
        int mid = sn[now].l + sn[now].r >> 1;
        SegNode ls, rs;
        if(l < mid && sn[now].lp != -1) ls = Query(sn[now].lp, l, r);
        if(r > mid && sn[now].rp != -1) rs = Query(sn[now].rp, l, r);
        return SegMerge(ls, rs);
    }
};
```


## 可持久化数据结构

### 可持久化线段树

灵活性方面和线段树一样，没有固定模板。这里以区间和为例。

动态内存版本，如果卡常，可以预分配内存例如`rt.reserve(maxn)`。一般`rt`分配`maxn`，`lc`、`rc`、`val`分配`maxn * 32`。

```cpp
struct PersistentSegTree
{
    std::vector<int> val, rt, lc, rc;
    void Init()
    {
        rt.clear(), lc.clear(), rc.clear(), val.clear();
    }
    int Add(int v=0, int l=-1, int r=-1)
    {
        val.push_back(v);
        lc.push_back(l);
        rc.push_back(r);
        return lc.size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(val[now], lc[now], rc[now]);
    }
    void Build()
    {
        rt.push_back(Add());
    }
    int Update(int now, int left, int right, int v, int loc)
    {
        int nex = Clone(now);
        val[nex] += v;
        if(left < right - 1)
        {
            int mid = left + right >> 1;
            if(loc < mid) {int nlc = Update(lc[nex], left, mid, v, loc); lc[nex] = nlc;}
            else {int nrc = Update(rc[nex], mid, right, v, loc); rc[nex] = nrc;}
        }
        return nex;
    }
    int Query(int now, int l, int r, int left, int right)
    {
        if(now == -1) return 0;
        if(l <= left && r >= right) return val[now];
        int mid = left + right >> 1, ret = 0;
        if(l < mid) ret += Query(lc[now], l, r, left, mid);
        if(r > mid) ret += Query(rc[now], l, r, mid, right);
        return ret;
    }
};
```

静态构建，以可持久化数组为例：

```cpp
    int _Build(int left, int right, std::vector<int> &a)
    {
        if(left >= right) return -1;
        int now = Add();
        if(left == right - 1)
        {
            val[now] = a[left];
        }
        else
        {
            int mid = left + right >> 1;
            int nlc = _Build(left, mid, a);
            lc[now] = nlc;
            int nrc = _Build(mid, right, a);
            rc[now] = nrc;
        }
        return now;
    }
    void Build(int left, int right, int std::vector<int> &a)
    {
        rt.push_back(_Build(left, right, a));
    }
```

版本差异查询，以区间第 k 小为例：

```cpp
    int Query(int lnow, int rnow, int left, int right, int kth)
    {
        if(left == right - 1) return left;
        int sum = val[lc[rnow]] - val[lc[lnow]];
        int mid = left + right >> 1;
        if(sum >= kth) return Query(lc[lnow], lc[rnow], left, mid, kth);
        else return Query(rc[lnow], rc[rnow], mid, right, kth - sum);
    }
```

使用示例：

```cpp
PersistentSegTree pst;
pst.Init();
pst.Build();
for(int i = 0; i < n; i ++)
    pst.rt.push_back(pst.Update(pst.rt.back(), 0, n, val, loc);
```

### 可持久化Trie

```cpp

struct PersistentTrie
{
    std::vector<int> rt, ch[2], sz;
    const int MBIT = 33;
    void Init()
    {
        rt.clear(), ch[0].clear(), ch[1].clear(), sz.clear();
        rt.push_back(Add());
    }
    int Add(int s=0, int l=-1, int r=-1)
    {
        sz.push_back(s);
        ch[0].push_back(l);
        ch[1].push_back(r);
        return ch[0].size() - 1;
    }
    int Clone(int now)
    {
        if(now == -1) return Add();
        return Add(sz[now], ch[0][now], ch[1][now]);
    }
    void Insert(int nv, LL x)
    {
        int nex = Clone(nv);
        sz[nex] ++;
        rt.push_back(nex);
        for(int i = MBIT; i >= 0; i --)
        {
            int dir = x >> i & 1;
            int newNex = Clone(ch[dir][nex]);
            sz[newNex] ++;
            ch[dir][nex] = newNex;
            nex = ch[dir][nex];
        }
    }
    inline int Sz(int now){return now == -1 ? 0 : sz[now];}
    inline int Ch(int dir, int now){return now == -1 ? -1 : ch[dir][now];}
};
```

以 x 与 Trie 的 nv 版本异或第 k 大为例的`Query`

```cpp
    LL Query(int nv, LL x, int k)
    {
        LL res = 0;
        if(Sz(nv) < k) return -1;
        for(int i = MBIT; i >= 0; i --)
        {
            int dir = x >> i & 1;
            if(Sz(Ch(!dir, nv)) >= k)
                nv = Ch(!dir, nv), res = res << 1 | 1;
            else
                k -= Sz(Ch(!dir, nv)), nv = Ch(dir, nv), res <<= 1;
        }
        return res;
    }
    // pt.Query(pt.rt.back(), x, k);
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
    int Insert(KD_CO x_, KD_CO y_, KD_V v_=0);
    int Insert(vector<KD_CO> &co, KD_V v_=0);
    int _Insert(int &now, int last);
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

### KD树 K 近点

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

### KD树 K 远点

```cpp
typedef double DIS_V;
DIS_V RecDis(vector<KD_CO> &a, vector<KD_CO> &cl, vector<KD_CO> &ch)
{
    DIS_V ret = 0;
    for(int i = 0; i < a.size(); i ++)
        ret += max(Sqr(a[i] - cl[i]), Sqr(a[i] - ch[i]));
    return sqrt(ret);
}
int KDTree::_SearchK(int now, vector<KD_CO> &p, int k, pqueue &q)
{
    if(now == -1) return 0;
    DIS_V lrecdis = Gid(lc, now) == -1 ? 0 : RecDis(p, cl[lc[now]], ch[lc[now]]);
    DIS_V rrecdis = Gid(rc, now) == -1 ? 0 : RecDis(p, cl[rc[now]], ch[rc[now]]);
    DIS_V &firstdis = lrecdis > rrecdis ? lrecdis : rrecdis;
    DIS_V &seconddis = lrecdis > rrecdis ? rrecdis : lrecdis;
    vector<int> &firstdir = lrecdis > rrecdis ? lc : rc;
    vector<int> &seconddir = lrecdis > rrecdis ? rc : lc;
    if(q.size() < k || abs(q.top().first) <= firstdis)
        _SearchK(Gid(firstdir, now), p, k, q);
    DIS_V dis = Dis(p, ax[now]);
    if(q.size() < k || abs(q.top().first) <= dis)
    {
        q.push(pii(-dis, now));  // 距离小、编号大的放堆顶，距离存负数省的写operator
        while(q.size() > k) q.pop();
    }
    if(q.size() < k || abs(q.top().first) <= seconddis)
        _SearchK(Gid(seconddir, now), p, k, q);
    return 0;
}
```

### KD树 K 近曼哈顿距离点

```cpp
typedef pair<int, int> pii;
typedef priority_queue<pii> pqueue;
typedef int KD_CO;
typedef int KD_V;
double Sqr(double x){return x * x;}
struct KDTree
{
    // 求曼哈顿距离最近 k 个点
    int SearchK(vector<KD_CO> &p, int k, pqueue &q){return _SearchK(root, p, k, q);}
    int _SearchK(int now, vector<KD_CO> &p, int k, pqueue &q);
};
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


## 数组

### Lower Bound

```cpp
int LowerBound(int a[], int n, int q)
{
    int left = 0, right = n, mid;
    while(left < right)
    {
        mid = left + right >> 1;
        if(a[mid] >= q) right = mid;
        else left = mid + 1;
    }
    return left;
}
```