### `最长异或路径`

题意：树上一段路径的边权异或结果最大的值

巧思：一个数异或两次相当于没有，那么任意两点路径的异或值，与这两点到根的路径异或值再异或的结果相同

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
struct Tree {
    std::vector<int> fst, nex, v, w;
    void Init(int n_=0) {
        fst.clear(); 
        if(n_) fst.resize(n_ + 1, -1), 
        nex.clear(), v.clear(), w.clear();
    }
    void AddEdge(int u_, int v_, int w_) {
        while(fst.size() <= u_) fst.push_back(-1);
        while(fst.size() <= v_) fst.push_back(-1);
        v.push_back(v_);
        nex.push_back(fst[u_]);
        w.push_back(w_);
        fst[u_] = nex.size() - 1;
    }
    void DbEdge(int u_, int v_, int w_){AddEdge(u_, v_, w_); AddEdge(v_, u_, w_);}
};
struct Trie {
    std::vector<int> rt, ch[2];
    const int MBIT = 31;
    void Init() {
        rt.clear(), ch[0].clear(), ch[1].clear();
        Add();
    }
    int Add() {
        ch[0].push_back(-1);
        ch[1].push_back(-1);
        return Last();
    }
    int Last() {
        return ch[0].size() - 1;
    }
    void Insert(int x) {
        int now = 0;
        for(int i = MBIT; i >= 0; i --) {
            int ith1 = x >> i & 1;
            if(ch[ith1][now] == -1) {
                ch[ith1][now] = Add();
            }
            now = ch[ith1][now];
        }
    }
    int Query(int x) {
        int now = 0, ret = 0;
        for(int i = MBIT; i >= 0; i --) {
            int ith1 = x >> i & 1;
            if(ch[ith1 ^ 1][now] != -1) {
                ret |= 1 << i;
                now = ch[ith1 ^ 1][now];
            } else {
                now = ch[ith1][now];
            }
        }
        return ret;
    }
};
Tree te;
Trie ti;
int n, ans;
std::vector<bool> vis;
void DFS(int now, int val) {
    vis[now] = true;
    ti.Insert(val);
    ans = std::max(ans, ti.Query(val));
    for(int i = te.fst[now]; i != -1; i = te.nex[i]) {
        if(vis[te.v[i]]) continue;
        DFS(te.v[i], val ^ te.w[i]);
    }
}
int main() {
    int a, b, c;
    while(scanf("%d", &n) != EOF) {
        vis.clear();
        vis.resize(n + 1);
        std::fill(vis.begin(), vis.end(), 0);
        te.Init(n);
        for(int i = 1; i < n; i ++) {
            scanf("%d%d%d", &a, &b, &c);
            te.DbEdge(a, b, c);
        }
        ans = 0;
        ti.Init();
        DFS(1, 0);
        printf("%d\n", ans);
    }
    return 0;
}
```