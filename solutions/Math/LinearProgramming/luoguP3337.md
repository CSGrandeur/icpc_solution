### `[ZJOI2013] 防守战线`

> difficulty: 3

求对偶就刚好是最大标准型，`Pivot`时`a[i][l] > -eps && a[i][l] < eps`条件比较重要，不然可能超时

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<vector>
const double eps = 1e-7;
const double inf = 1e20;

struct Simplex {
    // 标准型： max Σcx, s.t. ax<=b, x>=0， m 约束条件个数， n 自由变量个数
    std::vector<double> b, c;
    std::vector<std::vector<double> >a;
    double z;
    int m, n;
    void Init(int m_, int n_) {
        m = m_, n = n_, z = 0;
        a.resize(m + 10, std::vector<double>(n + 10, 0));
        b.resize(m + 10, 0);
        c.resize(n + 10, 0);
    }
    void Pivot(int l, int e) {
        b[e] /= a[e][l];
        for(int j = 0; j < n; j ++)
            if(j != l) a[e][j] /= a[e][l];
        a[e][l] = 1 / a[e][l];
        for(int i = 0; i < m; i ++) {
            if(i == e || a[i][l] > -eps && a[i][l] < eps) continue;
            b[i] -= a[i][l] * b[e];
            for(int j = 0; j < n; j ++)
                if(j != l) a[i][j] -= a[i][l] * a[e][j];
            a[i][l] = -a[i][l] * a[e][l];
        }
        z += c[l] * b[e];
        for(int j = 0; j < n; j ++)
            if(j != l) c[j] -= c[l] * a[e][j];
        c[l] *= -a[e][l];
    }
    double Solve() {
        while(true) {
            int l = -1, e = -1;
            double maxc = eps;
            for(int j = 0; j < n; j ++)
                if(c[j] > maxc) {
                    maxc = c[l = j];
                }
            if(l == -1) return z;
            double minba = inf;
            for(int i = 0; i < m; i ++)
                if(a[i][l] > eps && minba > b[i] / a[i][l]) {
                    minba = b[i] / a[i][l];
                    e = i;
                }
            if(e == -1) return inf;
            Pivot(l, e);
        }
    }
};
void BuildSimple(Simplex &spx, int n, int m, std::vector<double> &C, std::vector<int> &L, std::vector<int> &R, std::vector<double> &D) {
    spx.Init(n, m);
    for(int i = 0; i < n; i ++) {
        for(int j = 0; j < m; j ++) {
            spx.a[i][j] = i >= L[j] - 1 && i <= R[j] - 1;
        }
        spx.b[i] = C[i];
    }
    for(int j = 0; j < m; j ++) {
        spx.c[j] = D[j];
    }
}
Simplex spx;
int n, m;
std::vector<int> L, R;
std::vector<double> C, D;
int main() {
    
    int s, t, c;
    while(scanf("%d%d", &n, &m) != EOF){
        C.resize(n);
        L.resize(m); R.resize(m); D.resize(m);
        for(int i = 0; i < n; i ++)
            scanf("%lf", &C[i]);
        for(int i = 0; i < m; i ++)
            scanf("%d%d%lf", &L[i], &R[i], &D[i]);
        BuildSimple(spx, n, m, C, L, R, D);
        printf("%d\n", (int)(spx.Solve() + eps));
    }
    return 0;
}
```