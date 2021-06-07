# 数学

## 筛素数

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

## 高斯消元

### 整数解

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

### 浮点解

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

### 异或方程组

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