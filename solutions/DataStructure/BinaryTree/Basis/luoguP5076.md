### `【深基16.例7】普通二叉树（简化版）`

> difficulty: 2

二叉树基本操作

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
const int maxn = 1e5 + 10;
struct Node {
    int data;
    int cnt;
    int l, r;
    void Init(){l = r = -1; cnt = 0;}
    void Init(int d_, int c_){data = d_; cnt = c_; Init();}
};
Node nd[maxn];
int n, tp, rt;
inline int Lnum(int now){return nd[now].l == -1 ? 0 : nd[nd[now].l].cnt;}
inline int Rnum(int now){return nd[now].r == -1 ? 0 : nd[nd[now].r].cnt;}
int QueryRank(int now, int x, int lsum=0) {
    if(now == -1) {
        return lsum + 1;
    }
    if(x == nd[now].data) {
        return lsum + Lnum(now) + 1;
    }
    if(x < nd[now].data) {
        return QueryRank(nd[now].l, x, lsum);
    }
    return QueryRank(nd[now].r, x, lsum + nd[now].cnt - Rnum(now));
}
int QueryByRank(int now, int rk, int lsum=0) {
    if(rk - lsum <= Lnum(now)) {
        return QueryByRank(nd[now].l, rk, lsum);
    }
    if(rk - lsum <= nd[now].cnt - Rnum(now)) {
        return nd[now].data;
    }
    return QueryByRank(nd[now].r, rk, lsum + nd[now].cnt - Rnum(now));
}
int Find(int now, int x) {
    if(x == nd[now].data) {
        return now;
    }
    return x < nd[now].data ? nd[now].l : nd[now].r;
}
int QueryPre(int now, int x, int last=-2147483647) {
    if(now == -1) return last;
    if(x < nd[now].data) {
        return QueryPre(nd[now].l, x, last);
    } else if(x > nd[now].data) {
        return QueryPre(nd[now].r, x, nd[now].data);
    }
    if(nd[now].l == -1) {
        return last;
    }
    int pnow = nd[now].l;
    for(; nd[pnow].r != -1; pnow = nd[pnow].r);
    return nd[pnow].data;
}

int QueryNex(int now, int x, int last=2147483647) {
    if(now == -1) return last;
    if(x < nd[now].data) {
        return QueryNex(nd[now].l, x, nd[now].data);
    } else if(x > nd[now].data) {
        return QueryNex(nd[now].r, x, last);
    }
    if(nd[now].r == -1) {
        return last;
    }
    int pnow = nd[now].r;
    for(; nd[pnow].l != -1; pnow = nd[pnow].l);
    return nd[pnow].data;
}
void Insert(int &now, int x) {
    if(now == -1) {
        nd[tp].Init(x, 0);
        now = tp ++;
    } else if(x < nd[now].data) {
        Insert(nd[now].l, x);
    } else if(x > nd[now].data){
        Insert(nd[now].r, x);
    }
    nd[now].cnt ++;
}
int main() {
    int op, x;
    while(scanf("%d", &n) != EOF) {
        tp = 0;
        rt = -1;
        for(int i = 0; i < n; i ++) {
            scanf("%d%d", &op, &x);
            switch (op)
            {
                case 1: printf("%d\n", QueryRank(rt, x)); break;
                case 2: printf("%d\n", QueryByRank(rt, x)); break;
                case 3: printf("%d\n", QueryPre(rt, x)); break;
                case 4: printf("%d\n", QueryNex(rt, x)); break;
                case 5: Insert(rt, x); break;
            }
        }

    }
}
```