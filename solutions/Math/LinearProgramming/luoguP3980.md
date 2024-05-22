### `[NOI2008] 志愿者招募`

> difficulty: 3

处理为对偶问题，即最大标准型

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
    void Pivot(int e, int l) {
        b[l] /= a[l][e];
        for(int j = 0; j < n; j ++)
            if(j != e) a[l][j] /= a[l][e];
        a[l][e] = 1 / a[l][e];
        for(int i = 0; i < m; i ++) {
            if(i == l || a[i][e] > -eps && a[i][e] < eps) continue;
            b[i] -= a[i][e] * b[l];
            for(int j = 0; j < n; j ++)
                if(j != e) a[i][j] -= a[i][e] * a[l][j];
            a[i][e] = -a[i][e] * a[l][e];
        }
        z += c[e] * b[l];
        for(int j = 0; j < n; j ++)
            if(j != e) c[j] -= c[e] * a[l][j];
        c[e] *= -a[l][e];
    }
    double Solve() {
        while(true) {
            int e = -1, l = -1;
            double maxc = eps;
            for(int j = 0; j < n; j ++)
                if(c[j] > maxc) {
                    maxc = c[e = j];
                }
            if(e == -1) return z;
            double minba = inf;
            for(int i = 0; i < m; i ++)
                if(a[i][e] > eps && minba > b[i] / a[i][e]) {
                    minba = b[i] / a[i][e];
                    l = i;
                }
            if(l == -1) return inf;
            Pivot(e, l);
        }
    }
};
void BuildSimple(Simplex &spx, int N, int M, std::vector<int> &A, std::vector<int> &L, std::vector<int> &R, std::vector<int> &C) {
    spx.Init(M, N);
    for(int i = 0; i < M; i ++) {
        for(int j = 0; j < N; j ++) {
            spx.a[i][j] = j >= L[i] - 1 && j <= R[i] - 1;
        }
        spx.b[i] = C[i];
    }
    for(int j = 0; j < N; j ++) {
        spx.c[j] = A[j];
    }
}
Simplex spx;
int N, M;
std::vector<int> L, R, A, C;
int main() {
    while(scanf("%d%d", &N, &M) != EOF){
        A.resize(N);
        C.resize(M); L.resize(M); R.resize(M); 
        for(int i = 0; i < N; i ++)
            scanf("%d", &A[i]);
        for(int i = 0; i < M; i ++)
            scanf("%d%d%d", &L[i], &R[i], &C[i]);
        BuildSimple(spx, N, M, A, L, R, C);
        printf("%d\n", (int)(spx.Solve() + eps));
    }
    return 0;
}
```

