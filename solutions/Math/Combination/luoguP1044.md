### `[NOIP2003 普及组] 栈`

> difficulty: 2

卡特兰数。

证明：设 x 为当前出栈序列的最后一个，则x有n种取值

比 x 小的数出栈情况数 h(x-1)

比 x 大的数出栈情况数 h(n-x)

每个数最后出栈的情况数相加，与卡特兰数递归公式一致

代码用卡特兰数组合数公式完成

```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef long long LL;
const int maxn = 40;
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
LL Cattelan(int n)
{
    return cm[n << 1][n] - cm[n << 1][n - 1];
}
int main()
{
    int n;
    ComList();
    while(scanf("%d", &n) != EOF)
        printf("%lld\n", Cattelan(n));
    return 0;
}
```

