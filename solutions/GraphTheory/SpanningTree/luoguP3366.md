### `最小生成树`

> difficulty: 2

最小生成树模板，数据范围卡掉了邻接矩阵

#### Prim

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<queue>
const int maxn = 5e3 + 10;
const int maxm = 4e5 + 10;
int fst[maxn], v[maxm], w[maxm], nex[maxm];
int key[maxn], intree[maxn];
int tp, n, m;
void AddEdge(int x, int y, int z) {
    v[tp] = y;
    w[tp] = z;
    nex[tp] = fst[x];
    fst[x] = tp;
    tp ++;
}
void DbEdge(int x, int y, int z) {
    AddEdge(x, y, z);
    AddEdge(y, x, z);
}
int Prim(int start) {
    memset(key, 0x3f, sizeof(key));
    memset(intree, 0, sizeof(intree));
    key[start] = 0;
    int res = 0;
    for(int i = 0; i < n; i ++) {
        int now = -1;
        for(int j = 1; j <= n; j ++) {
            if(!intree[j] && (now == -1 || key[j] < key[now])) {
                now = j;
            }
        }
        if(now == -1 || key[now] == 0x3f3f3f3f) {
            return -1;
        }
        res += key[now];
        intree[now] = 1;
        for(int j = fst[now]; j != -1; j = nex[j]) {
            if(w[j] < key[v[j]]) {
                key[v[j]] = w[j];
            }
        }
    }
    return res;
}
int main() {
    int x, y, z;
    while(scanf("%d%d", &n, &m) != EOF) {
        tp = 0;
        memset(fst, -1, sizeof(fst));
        for(int i = 0; i < m; i ++) {
            scanf("%d%d%d", &x, &y, &z);
            DbEdge(x, y, z);
        }
        int res = Prim(1);
        if(res == -1) {
            printf("orz\n");
        } else {
            printf("%d\n", res);
        }
    }
    return 0;
}
```

#### Prim 堆优化

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<queue>
#include<functional>
typedef std::pair<int, int> pii;
const int maxn = 5e3 + 10;
const int maxm = 4e5 + 10;
int fst[maxn], v[maxm], w[maxm], nex[maxm];
int key[maxn], intree[maxn];
int tp, n, m;
void AddEdge(int x, int y, int z) {
    v[tp] = y;
    w[tp] = z;
    nex[tp] = fst[x];
    fst[x] = tp;
    tp ++;
}
void DbEdge(int x, int y, int z) {
    AddEdge(x, y, z);
    AddEdge(y, x, z);
}
int Prim(int start) {
    memset(key, 0x3f, sizeof(key));
    memset(intree, 0, sizeof(intree));
    key[start] = 0;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > q;
    q.push(pii(0, start));
    int res = 0, cnt = 0;
    while(!q.empty()) {
        int now = q.top().second, oldkey = q.top().first;
        q.pop();
        if(intree[now] || key[now] < oldkey) {
            continue;
        }
        res += key[now];
        intree[now] = true;
        cnt ++;
        for(int j = fst[now]; j != -1; j = nex[j]) {
            if(w[j] < key[v[j]]) {
                key[v[j]] = w[j];
                q.push(pii(w[j], v[j]));
            }
        }
    }
    return cnt == n ? res : -1;
}
int main() {
    int x, y, z;
    while(scanf("%d%d", &n, &m) != EOF) {
        tp = 0;
        memset(fst, -1, sizeof(fst));
        for(int i = 0; i < m; i ++) {
            scanf("%d%d%d", &x, &y, &z);
            DbEdge(x, y, z);
        }
        int res = Prim(1);
        if(res == -1) {
            printf("orz\n");
        } else {
            printf("%d\n", res);
        }
    }
    return 0;
}
```

#### Kruskal

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
const int maxn = 5e3 + 10;
const int maxm = 4e5 + 10;
int fst[maxn], u[maxm], v[maxm], nex[maxm], w[maxm];
int p[maxn], idx[maxm];
int tp, n, m;
void AddEdge(int a, int b, int c) {
    u[tp] = a;
    v[tp] = b;
    w[tp] = c;
    nex[tp] = fst[a];
    fst[a] = tp;
    tp ++;
}
void DbEdge(int a, int b, int c) {
    AddEdge(a, b, c);
    AddEdge(b, a, c);
}
int fa(int x) {
    return x == p[x] ? x : (p[x] = fa(p[x]));
}
bool cmp(const int &a, const int &b) {
    return w[a] < w[b];
}
int Kruscal() {
    for(int i = 0; i <= n; i ++) {
        p[i] = i;
    }
    for(int i = 0; i < tp; i ++) {
        idx[i] = i;
    }
    std::sort(idx, idx + tp, cmp);
    int ret = 0, cnt = 0;
    for(int i = 0; i < tp; i ++) {
        int ed = idx[i];
        if(fa(u[ed]) == fa(v[ed])) {
            continue;
        }
        p[fa(u[ed])] = fa(v[ed]);
        ret += w[ed];
        cnt ++;
    }
    return cnt == n - 1 ? ret : -1;
}
int main() {
    int a, b, c;
    while(scanf("%d%d", &n, &m) != EOF && n) {
        tp = 0;
        memset(fst, -1, sizeof(fst));
        for(int i = 0; i < m; i ++) {
            scanf("%d%d%d", &a, &b, &c);
            DbEdge(a, b, c);
        }
        int ret = Kruscal();
        if(ret == -1) {
            printf("orz\n");
        } else {
            printf("%d\n",  ret);
        }
    }
    return 0;
}
```