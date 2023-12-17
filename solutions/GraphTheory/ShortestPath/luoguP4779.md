### `单源最短路径（标准版）`

> difficulty: 1

Dijkstra入门. 注意题目提供的是有向边，别建成双向了.

```cpp
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<queue>
#include<functional>
typedef std::pair<int, int> pii;
const int maxn = 1e5 + 10;
const int maxm = 4e5 + 10;
int fst[maxn], v[maxm], nex[maxm], w[maxm];
int key[maxn];
int tp, n, m, s;
void AddEdge(int a, int b, int c) {
    v[tp] = b;
    w[tp] = c;
    nex[tp] = fst[a];
    fst[a] = tp;
    tp ++;
}
void Dijkstra() {
    memset(key, 0x3f, sizeof(key));
    key[s] = 0;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > q;
    q.push(pii(0, s));
    while(!q.empty()) {
        int now = q.top().second;
        int oldkey = q.top().first;
        q.pop();
        if(key[now] < oldkey) {
            continue;
        }
        for(int i = fst[now]; i != -1; i = nex[i]) {
            if(key[now] + w[i] < key[v[i]]) {
                key[v[i]] = key[now] + w[i];
                q.push(pii(key[v[i]], v[i]));
            }
        }
    }
}
int main() {
    int a, b, c;
    while(scanf("%d%d%d", &n, &m, &s) != EOF) {
        tp = 0;
        memset(fst, -1, sizeof(fst));
        for(int i = 0; i < m; i ++) {
            scanf("%d%d%d", &a, &b, &c);
            AddEdge(a, b, c);
        }
        Dijkstra();
        for(int i = 1; i <= n; i++) {
            printf(" %d" + (i == 1), key[i]);
        }
        printf("\n");
    }
    return 0;
}
```