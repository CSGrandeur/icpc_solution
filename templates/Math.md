# 数学


## 组合数

### 组合数基本递推

```cpp
typedef long long LL;
LL cm[maxn][maxn];
void ComList()
{
    for(int i = 0; i < maxn; i ++)
        for(int j = 0; j <= i; j ++)
        {
            if(j > i - j) cm[i][j] = cm[i][i - j];
            else if(!j) cm[i][j] = 1;
            else cm[i][j] = j == 1 ? i : cm[i - 1][j] + cm[i - 1][j - 1];
        }
}
```



## 数列

### 卡特兰数

**公式1：递归**

初始化 $h(0)=h(1)=1$

$$
h(n)=\sum_{i=0}^{n-1}h(i)*h(n-i-1)
$$

**公式2：递推**

$$
h(n)=h(n-1)*(4*n-2)/(n+1)
$$

**公式3：组合数公式1**

$$
h(n)=C_{2n}^{n}/(n+1),(n=0,1,2,...)
$$

**公式4：组合数公式2**，推荐

$$
h(n)=C_{2n}^{n}-C_{2n}^{n-1},(n=0,1,2,...)
$$

### 斯特林数

#### 第二类斯特林数

将 n 个互不相同的元素划分为 k 个互不区分的非空子集的方案数，

递推式（可从新插入一个数的分类讨论证明）：

$$
S_{n}^{k} = S_{n - 1}^{k - 1} + k*S_{n - 1}^{k}
$$

通项公式（可用容斥原理证明）：

$$
S_{n}^{m} = \sum_{i=0}^{m}\frac{(-1)^{m-i}i^{n}}{i!(m-i)!}
$$

#### 第一类斯特林数

将 n 个互不相同的元素划分为 k 个互不区分的非空轮换的方案数。

一个轮换就是一个首尾相接的环形排列，两个可以通过旋转而互相得到的轮换是等价的。

递推式：

$$
S_{n}^{k}=S_{n-1}^{k-1}+(n-1)S_{n-1}^{k}
$$

通项公式： 第一类斯特林数没有实用的通项公式。





## 筛质数

### 埃氏筛法

埃拉托斯特尼筛法（sieve of Eratosthenes）

`O(nloglogn)`

```cpp
bool isPrime[maxn];
void SetPrime()
{
    memset(isPrime, 1, sizeof(isPrime));
    isPrime[0] = isPrime[1] = false;
    for(int i = 2; i * i < maxn; i ++)
    {
        if(!isPrime[i])
            continue;
        for(int j = i * i; j < maxn; j += i)
            isPrime[j] = false;
    }
}
```

### 欧拉线性筛法

`O(n)`

```cpp
bool isPrime[maxn];
int prime[maxm], ptp
void SetPrime()
{
    ptp = 0;
    memset(isPrime, 1, sizeof(isPrime));
    isPrime[0] = isPrime[1] = false;
    for(int i = 2; i < maxn; i ++)
    {
        if(isPrime[i]) prime[ptp ++] = i;
        for(int j = 0; j < ptp && i * prime[j] < maxn; j ++)
        {
            isPrime[i * prime[j]] = false;
            if(i % prime[j] == 0) break;
        }
    }
}
```

## 欧拉函数

### 直接计算欧拉函数

```cpp
// 𝑛 \prod_(𝑝|𝑛) {(1-1/p)}
int Euler(int n)
{
    int res = n, tn = n;
    for(int i = 2; i * i <= tn; i ++)
    {
        if(tn % i) continue;
        res = res / i * (i - 1);
        while(tn % i == 0) tn /= i;
    }
    if(tn > 1) res = res / tn * (tn - 1);
    return res;
}
```

### 欧拉函数打表

```cpp
int el[maxn];   // 累加求和的话注意 long long
void EulerList()
{
    el[1] = 1;
    for(int i = 2; i < maxn; i ++)
        el[i] = i;
    for(int i = 2; i < maxn; i ++)
    {
        if(el[i] == i)
            for(int j = i; j < maxn; j += i)
                el[j] = el[j] / i * (i - 1);
    }
}
```

## 快速幂取模

```cpp
// const int mod = ...
int PowMod(int a, int n, int mod=mod)
{
    int ret = 1;
    for(; n; n >>= 1, a = 1LL * a * a % mod)
        if(n & 1) ret = 1LL * ret * a % mod;
    return ret;
}
```

## 乘法逆元

### 方法1：费马小定理

`GCD(a, b)==1`，则`a^(b-1) % b == 1`，故 `a^(b-2)`为`a`模`b`的逆元，调用快速幂取模。

模数 `b` 为质数时可用。

```cpp
int FermatInv(int a, int b)
{
    return PowMod(a, b - 2, b);
}
```

### 方法2：扩展欧几里得

求逆元模数b通常很大，用long long保险，可行解存在引用参数x,y中，返回值为GCD(a,b)的结果。

最常用、安全的求逆元方式。

```cpp
typedef long long LL;  
LL ExGCD(LL a, LL b, LL &x, LL &y)
{
    if(!b)
    {
        x = 1, y = 0;
        return a;
    }
    LL d = ExGCD(b, a % b, x, y), t = x;
    x = y, y = t - a / b * x;
    return d; 
}
```

```cpp
int ExGcdInv(int a, int mod)
{
    LL x, y;
    ExGCD(a, mod, x, y);
    return (x % mod + mod) % mod;
}
```

### 方法3：递归求逆元

`mod` 必须为质数。

```cpp
LL Inv(LL a, LL mod)
{
    if(a == 1) return 1;
    return (mod - mod / a) * Inv(mod % a) % mod;
}
```

### 逆元打表

```cpp
int invList[maxn];
void GetInvList(int mod)
{
    invList[1] = 1;
    for(int i = 2; i < maxn; i ++)
        invList[i] = 1LL * (mod - mod / i) * invList[mod % i] % mod;
}
```

## 线性代数

### 行列式计算

- 行列式与它的转置行列式相等
- 互换行列式两行（或列）位置，行列式的值变号
- 如果两列完全相同，则行列式值为0
- 行列式有两行（列）对应成比例，则行列式值为0
- 某列元素同乘 k，等于 k 乘行列式
- 某行（列）元素乘同一数后加到另一行（列）元素上去，行列式值不变

### 高斯消元

#### 整数解

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 110;
int a[maxn][maxn];
int x[maxn];
bool xFree[maxn];
int GCD(int a, int b){return b ? GCD(b, a % b) : a;}
inline int LCM(int a, int b) {return a / GCD(a, b) * b;}

int GaussEl(int eqn, int xn)
{
    // eqn: 方程数（行数），xn: 变量数（列数，不包含常数项）
    int k, maxRow, curCol = 0, frnum = 0;
    for(int i = 0; i < xn; i ++)
    {
        x[i] = 0;
        xFree[i] = true;
    }
    for(k = 0; k < eqn && curCol < xn; k ++, curCol ++)
    {
        maxRow = k;
        for(int i = k + 1; i < eqn; i ++)
            if(abs(a[i][curCol] > abs(a[maxRow][curCol])))
                maxRow = i;
        if(maxRow != k)
            for(int j = k; j <= xn; j ++)
                std::swap(a[k][j], a[maxRow][j]);
        if(a[k][curCol] == 0)
        {
            xFree[frnum ++] = curCol;
            k --;
            continue;
        }
        for(int i = k + 1; i < eqn; i ++)
        {
            if(!a[i][curCol]) continue;
            int iklcm = LCM(abs(a[i][curCol]), abs(a[k][curCol]));
            int ta = iklcm / abs(a[i][curCol]);
            int tb = iklcm / abs(a[k][curCol]);
            if(a[i][curCol] * a[k][curCol] < 0) tb = -tb;
            for(int j = curCol; j <= xn; j ++)
                a[i][j] = a[i][j] * ta - a[k][j] * tb;
        }
    }
    // 无解
    for(int i = k; i < eqn; i ++) if(a[i][curCol]) return -1;
    // 无穷解，返回自由变元个数
    if(k < xn) return xn - k;
    // 唯一解
    for(int i = xn - 1; i >= 0; i --)
    {
        int tmp = a[i][xn];
        for(int j = i + 1; j < xn; j ++)
            tmp -= a[i][j] * x[j];
        if(tmp % a[i][i]) return -2; // 浮点数解
        x[i] = tmp / a[i][i];
    }
    return 0;
}
```

#### 浮点解

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
const int maxn = 110;
const double eps = 1e-8;
double a[maxn][maxn], x[maxn];
bool xFree[maxn];

int GaussEl(int eqn, int xn)
{
    int curCol = 0, k = 0, frnum = 0;
    for(; k < eqn && curCol < xn; k ++, curCol ++)
    {
        int maxRow = k;
        for(int i = k + 1; i < eqn; i ++)
            if(abs(a[i][curCol]) > abs(a[maxRow][curCol]))
                maxRow = i;
        if(abs(a[maxRow][curCol]) < eps) 
        {
            xFree[frnum ++] = curCol;
            k --;
            continue;
        }
        if(maxRow != k) 
            for(int i = curCol; i <= xn; i ++)
                std::swap(a[k][i], a[maxRow][i]);
        for(int i = k + 1; i < eqn; i ++)
        {
            if(abs(a[i][curCol]) > eps)
            {
                double tmp = a[i][curCol] / a[k][curCol];
                for(int j = curCol; j <= xn; j ++)
                    a[i][j] -= a[k][j] * tmp;
                a[i][curCol] = 0;
            }
        }
    }
    for(int i = k; i < eqn; i ++) if(abs(a[i][xn]) > eps) return -1;
    if(k < xn) return xn - k;
    for(int i = xn - 1; i >= 0; i --)
    {
        double tmp = a[i][xn];
        for(int j = i + 1; j < xn; j ++)
            tmp -= x[j] * a[i][j];
        x[i] = tmp / a[i][i];
    }
    return 0;
}
```

#### 异或方程组

```cpp
#include<algorithm>
#include<bitset>
const int maxn = 110;
std::bitset<maxn> a[maxn], x;
int GaussXor(int eqn, int xn)
{
    int curCol = 0, k = 0;
    for(; k < eqn && curCol < xn; k ++, curCol ++)
    {
        int maxRow = k;
        for(; maxRow < eqn && !a[maxRow][curCol]; maxRow ++);
        if(maxRow == eqn) continue;
        if(maxRow != k) std::swap(a[k], a[maxRow]);
        for(int i = 0; i < eqn; i ++)
            if(i != k && a[i][curCol]) a[i] ^= a[k];
    }
    for(int i = k; i < eqn; i ++) if(a[i][xn]) return -1;
    if(k < xn) return xn - k;
    for(int i = xn - 1; i >= 0; i --)
        x[i] = a[i][xn];
    return 0;
}
```

#### 结果取模方程组

取模则可利用逆元处理除法，不用担心整除问题，方便地消解为单位矩阵。

下述代码中`Inv(a, mod)`为取逆元。

```cpp
void AddMod(int &a, int b){(a += b) %= mod;}
int GaussElMod(int eqn, int xn, int mod)
{
    int k, nzRow, curCol = 0, frnum = 0;
    for(k = 0; k < eqn && curCol < xn; k ++, curCol ++)
    {
        for(nzRow = k; nzRow < eqn && !a[nzRow][curCol]; nzRow ++);
        if(nzRow == eqn) {k --; continue;}
        for(int j = k; j <= xn; j ++)
            std::swap(a[k][j], a[nzRow][j]);
        int kcInv = Inv(a[k][curCol], mod);
        for(int j = k; j <= xn; j ++)
            a[k][j] = 1LL * a[k][j] * kcInv % mod;
        for(int i = 0; i < eqn; i ++)
        {
            if(i == k) continue;
            int ta = a[i][curCol];
            for(int j = k; j <= xn; j ++)
                AddMod(a[i][j], mod - 1LL * a[k][j] * ta % mod);
        }
    }
    for(int i = k; i < eqn; i ++) if(a[i][curCol]) return -1;
    if(k < xn) return xn - k;
    for(int i = 0; i < eqn; i ++)
        x[i] = a[i][xn];
    return 0;
}
```

### 线性规划

#### 单纯形法

```cpp
// Init n 为变量数， m 为方程数，对应初始化a,b,c时也要注意
const int maxn = 1e3 + 10;
const int maxm = 100;
const double inf = 1e20;
struct Simplex
{
    // 标准型：min Σcx, s.t. ax=b, x>=0
    double a[maxm][maxn], b[maxm], c[maxn], z;
    int n, m;
    void Init(int n_, int m_)
    {
        n = n_, m = m_;
        memset(c, 0, sizeof(c));
        memset(a, 0, sizeof(a));
        z = 0;
    }
    void Pivot(int k, int l)
    {
        b[l] /= a[l][k];
        for(int j = 0; j < n; j ++)
            if(j != k) a[l][j] /= a[l][k];
        a[l][k] = 1 / a[l][k];
        for(int i = 0; i < m; i ++)
        {
            if(i != l && fabs(a[i][k]) > 0)
            {
                b[i] -= a[i][k] * b[l];
                for(int j = 0; j < n; j ++)
                    if(j != k) a[i][j] -= a[i][k] * a[l][j];
                a[i][k] = -a[i][k] * a[l][k];
            }
        }
        z += c[k] * b[l];
        for(int j = 0; j < n; j ++)
            if(j != k) c[j] -= c[k] * a[l][j];
        c[k] *= -a[l][k];
    }
    double Solve()
    {
        while(true)
        {
            int k = 0, l = 0;
            double minc = inf;
            for(int i = 0; i < n; i ++)
            {
                if(c[i] < minc)
                {
                    minc = c[i];
                    k = i;
                }
            }
            if(minc >= 0) return z;
            double minba = inf;
            for(int i = 0; i < m; i ++)
            {
                if(a[i][k] > 0 && minba > b[i] / a[i][k])
                {
                    minba = b[i] / a[i][k];
                    l = i;
                }
            }
            if(minba == inf) return inf;
            Pivot(k, l);
        }
    }
};
```



## 微分&差分

### 二阶差分求区间加等差数列

区间加等差数列可用线段树实现，但复杂度较高。如果是一系列操作之后进行静态查询，可用二阶差分。

`[l, r)` 左闭右开区间，执行 **两遍** `PreSum`可恢复更新后数组。

```cpp
void AddAS(int l, int r, int lfirst, int dif)
{
    d2[l] += lfirst;
    d2[l + 1] += dif - lfirst;
    d2[r] -= (r - l) * dif + lfirst;
    d2[r + 1] -= (l - r + 1) * dif - lfirst;
}
void PreSum(int n)
{
    for(int i = 1; i < n; i ++)
        d2[i] += d2[i - 1];
}
```

## 多项式

### 快速傅里叶变换（FFT）

将 $y = a_{0} + a_{1}x + a_{2}x^{2} + ... a_{n}x^{n}$ 理解为 $a_{i}$ 为未知数的方程，则 $n+1$ 个 $x$ 的合适取值构建 $n+1$ 个方程的方程组，可以唯一确定$a_{0}$到$a_{n}$的值。

这样的方程组可以表示成 $((x_{0}, y_{0}), (x_{1}, y_{1}), ... (x_{n}, y_{n}))$ 的数对向量，FFT就是把系数序列$a_{i}$转为数对序列的方法。

调用需把 $len$ 对齐到最近的 $2^{k}$，$y$ 补 0 。 `FFT()` 的 `on` 参数取 `1` 为正变换，取 `0` 为反变换。

做大数乘法时需四舍五入最终反变换的结果（`+0.5`后取整）。

```cpp
const double pi = acos(-1);
typedef std::complex<double> Complex;
void BitRevChange(Complex y[], int len) 
{
    // len should be 2^k
    std::vector<int> rev(len, 0);
    for (int i = 0; i < len; i ++) 
    {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; ++i) 
        if (i < rev[i]) std::swap(y[i], y[rev[i]]);
}
void FFT(Complex y[], int len, int on=1)
{
    // on == 1: DFT; on == -1: IDFT; len should be 2^k
    BitRevChange(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
        Complex wn(cos(2 * pi / h), on * sin(2 * pi / h));
        for(int j = 0; j < len; j += h)
        {
            Complex w(1, 0);
            for(int k = j; k < j + (h >> 1); k ++)
            {
                Complex u = y[k], t = w * y[k + (h >> 1)];
                y[k] = u + t;
                y[k + (h >> 1)] = u - t;
                w = w * wn;
            }
        }
    }
    if(on != -1) return;
    for(int i = 0; i < len; i ++) y[i].real(y[i].real() / len);
}
```

### 数论变换（NTT）

当多项式系数都是整数，且问题可以对大质数`p`取模处理（即使题目没要求取模，但数据范围够小、取模不影响结果）时，可利用原根的性质，把FFT中的单位根替换为原根，其余原理类似，模板也很相似。

NTT全部为整数运算，相对FFT精度更好、速度更快。

要求序列长度`n`是`p-1`的约数。经常使用的模数有 `998244353`、`1004535809`、`469762049`，这几个的原根都是 `3`。



```cpp
void BitRevChange(int y[], int len) 
{
    // len should be 2^k
    std::vector<int> rev(len, 0);
    for (int i = 0; i < len; i ++) 
    {
        rev[i] = rev[i >> 1] >> 1;
        if (i & 1) rev[i] |= len >> 1;
    }
    for (int i = 0; i < len; ++i) 
        if (i < rev[i]) std::swap(y[i], y[rev[i]]);
}
void NTT(int y[], int len, int on=1)
{
    // on == 1: NTT; on == -1: INTT; len should be 2^k
    BitRevChange(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
        int wn = PowMod(rog, (mod - 1) / h, mod);
        if(on == -1) wn = PowMod(wn, mod - 2, mod);
        for(int j = 0; j < len; j += h)
        {
            int w = 1;
            for(int k = j; k < j + (h >> 1); k ++)
            {
                int u = y[k], t = 1LL * w * y[k + (h >> 1)] % mod;
                y[k] = (u + t) % mod;
                y[k + (h >> 1)] = (u - t + mod) % mod;
                w = 1LL * w * wn % mod;
            }
        }
    }
    if(on != -1) return;
    for(int i = 0, leninv = PowMod(len, mod - 2, mod); i < len; i ++) 
        y[i] = 1LL * y[i] * leninv % mod;
}
int main()
{
    while(scanf("%d%d", &n, &m) != EOF)
    {
        for(int i = 0; i <= n; i ++) scanf("%d", &a[i]);  // n 阶多项式有 n + 1 个系数
        for(int i = 0; i <= m; i ++) scanf("%d", &b[i]);
        int len = 1;
        for(; len <= n + m; len <<= 1);
        for(int i = n + 1; i < len; i ++) a[i] = 0;
        for(int i = m + 1; i < len; i ++) b[i] = 0;
        NTT(a, len); NTT(b, len);
        for(int i = 0; i < len; i ++) a[i] = 1LL * a[i] * b[i] % mod;
        NTT(a, len, -1);
        for(int i = 0; i <= n + m; i ++) printf("%d%c", a[i], " \n"[i == n + m]);
    }
    return 0;
}
```

#### 基于NTT计算多项式变量0~n-1次幂代入

设多项式为 `f(x)`，如果 `w` 不是 `p` 的原根，但 `w ^ n % p == 1`，长度为`2`的幂，求 `f(w^0)`~`f(w^(n-1))`

可按NTT迭代公式进行，`wn` 和 `y[k + (h>>1)]` 的计算略有不同。

```cpp
void NTT(int y[], int len, int rog, int mod)
{
    BitRevChange(y, len);
    for(int h = 2; h <= len; h <<= 1)
    {
        int wn = PowMod(rog, len / h, mod);
        for(int j = 0; j < len; j += h)
        {
            int w = 1, wn2len = PowMod(wn, h >> 1, mod);
            for(int k = j; k < j + (h >> 1); k ++)
            {
                int u = y[k], t = 1LL * w * y[k + (h >> 1)] % mod;
                y[k] = (u + t) % mod;
                y[k + (h >> 1)] = (u + 1LL * t * wn2len) % mod;
                w = 1LL * w * wn % mod;
            }
        }
    }
}
```


## 高精度

更新中...

```cpp
struct BigInt
{
    typedef std::vector<long long> VCT;
    VCT x;
    const int unt = 1e8;
    void Init(){x.clear();}
    BigInt(){}
    BigInt(const long long &a){Add(a);}
    BigInt(const int &a){Add(a);}
    BigInt(const BigInt &b){x = b.x;}
    BigInt(VCT &v){x = v;}
    void Add(const long long &a)
    {
        long long cur = a;
        for(auto &it : x)
        {
            it += cur;
            cur = it / unt;
            it = it % unt;
        }
        while(cur) x.push_back(cur % unt), cur /= unt;
    }
    BigInt &operator+=(long long a){Add(a); return *this;}
    BigInt operator+(long long a){BigInt tmp(x); tmp += a; return tmp;}
    BigInt &operator=(long long a){x.clear(); Add(a); return *this;}
    void Add(const BigInt &b)
    {
        int i;
        long long cur = 0;
        const VCT &y = b.x;
        if(x.size() < y.size()) x.resize(y.size(), 0);
        for(i = 0; i < x.size(); i ++)
        {
            x[i] += cur + (i < y.size() ? y[i] : 0);
            cur = x[i] / unt;
            x[i] %= unt;
        }
        while(cur) x.push_back(cur % unt), cur /= unt;
    }
    BigInt &operator+=(const BigInt &b){Add(b); return *this;}
    BigInt operator+(const BigInt &b){BigInt tmp(x); tmp += b; return tmp;}
    BigInt &operator=(const BigInt &b){x = b.x; return *this;}
    operator long long(){return x[0];}
    void Print()
    {
        if(x.size() == 0) {printf("0"); return; }
        for(int i = x.size() - 1; i >= 0; i --)
            printf(i == x.size() - 1 ? "%lld" : "%08lld", x[i]);
    }
    void Print3c()
    {
        if(x.size() == 0) {printf("0"); return; }
        char *buf = new char[x.size() * 8 + 1], *p = buf;
        for(int i = 0, j = x.size() - 1; i < x.size(); i ++, j --)
            p += sprintf(p, i ? "%08lld" : "%lld", x[j]);
        for(int i = strlen(buf) - 1, j = 0; buf[j]; j ++, i --)
            printf(",%c" + (i % 3 != 2 || !j), buf[j]);
        delete []buf;
    }
};
```

