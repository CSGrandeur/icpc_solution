// difficulty: 4
// Narrow Components
// 丰富信息的线段树，左右两区间 merge 时处理四角连通性，两个并查集合并为一个并查集的操作。
// 两区间的答案个数和减去中间联通数，合并并查集后维护了新区间四角连通性。
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<vector>
#include<queue>
#include<utility>
const int maxn = 5e5 + 10;
int fa(int x, int p[]){return p[x] = x == p[x] ? x : fa(p[x], p);}
struct SegTree
{
    struct Node
    {
        int l[3], r[3], p[6], cnt;
        Node(){Init();}
        void Init()
        {
            l[0]=l[1]=l[2]=r[0]=r[1]=r[2]=cnt=0;
            for(int i = 0; i < 6; i ++) p[i] = i;
        }
    };
    std::vector<Node> nds;
    void Init(int n){nds.resize((n << 2) + 10);}
    void Build(int now, int l, int r, char b[][maxn]);
    Node Merge(Node a, Node b);
    Node Query(int now, int l, int r, int ql, int qr);
};
void SegTree::Build(int now, int l, int r, char b[][maxn])
{
    if(l >= r) return;
    Node &nd = nds[now];
    nd.Init();
    if(l == r - 1)
    {
        for(int i = 0; i < 3; i ++)
        {
            nd.cnt += (nd.l[i] = nd.r[i] = b[i][l] - '0');
            if(i > 0 && nd.l[i] && nd.l[i - 1]) nd.p[i] = nd.p[i - 1], nd.cnt --;
            nd.p[i + 3] = nd.p[i];
        }
        return;
    }
    int mid = l + r >> 1;
    Build(now << 1, l, mid, b);
    Build(now << 1 | 1, mid, r, b);
    nd = Merge(nds[now << 1], nds[now << 1 | 1]);
}
SegTree::Node SegTree::Merge(Node a, Node b)
{
    Node ret = a;
    ret.cnt += b.cnt;
    int ptmp[12];
    for(int i = 0; i < 6; i ++) ptmp[i] = ret.p[i];
    for(int i = 0; i < 6; i ++) ptmp[i + 6] = b.p[i] + 6;
    for(int i = 0; i < 3; i ++)
        if(ret.r[i] && b.l[i] && fa(i + 3, ptmp) != fa(i + 6, ptmp))
            ptmp[fa(i + 3, ptmp)] = fa(i + 6, ptmp), ret.cnt --;
    for(int i = 0; i < 3; i ++)
    {
        ret.r[i] = b.r[i];
        if(fa(i, ptmp) > 2 && fa(i, ptmp) < 9) ptmp[fa(i, ptmp)] = i, ptmp[i] = i;
        if(fa(i + 9, ptmp) > 2 && fa(i + 9, ptmp) < 9) ptmp[fa(i + 9, ptmp)] = i + 9, ptmp[i + 9] = i + 9;
        ret.p[i] = fa(i, ptmp) >= 9 ? fa(i, ptmp) - 6 : fa(i, ptmp);
        ret.p[i + 3] = fa(i + 9, ptmp) >= 9 ? fa(i + 9, ptmp) - 6 : fa(i + 9, ptmp);
    }
    return ret;
}
SegTree::Node SegTree::Query(int now, int l, int r, int ql, int qr)
{
    if(ql <= l && qr >= r) return nds[now];
    int mid = l + r >> 1;
    if(ql < mid && qr > mid) return Merge(Query(now << 1, l, mid, ql, qr), Query(now << 1 | 1, mid, r, ql, qr));
    else if(ql < mid) return Query(now << 1, l, mid, ql, qr);
    else if(qr > mid) return Query(now << 1 | 1, mid, r, ql, qr);
}
char buf[3][maxn];
SegTree st;
int main()
{
    int n, q, l, r;
    while(scanf("%d", &n) != EOF)
    {
        for(int i = 0; i < 3; i ++) scanf("%s", buf[i]);
        st.Init(n);
        st.Build(1, 0, n, buf);
        scanf("%d", &q);
        while(q --)
        {
            scanf("%d%d", &l, &r);
            printf("%d\n", st.Query(1, 0, n, l - 1, r).cnt);
        }
    }
    return 0;
}