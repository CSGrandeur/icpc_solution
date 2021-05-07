# 数据结构

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
