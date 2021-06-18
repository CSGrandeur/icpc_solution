# 数学

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

## 快速幂取模

```cpp
int PowMod(int a, int n, int mod)
{
    int ret = 1;
    while(n)
    {
        if(n & 1) ret = 1LL * ret * a % mod;
        a = 1LL * a * a % mod;
        n >>= 1;
    }
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
    //
    if(b == 0)
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
    int x, y;
    ExGCD(a, b, x, y);
    return x;
}
```

### 方法3：递归求逆元

`mod` 必须为质数。

```cpp
LL Inv(LL a, LL mod)
{
    if(a == 1) return 1;
    return (mod - mod / a) * Inv(mod % i) % mod;
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
