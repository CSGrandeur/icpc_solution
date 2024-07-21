### `我家的门牌号`

> difficulty: 1

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
int Sum(int x) {    // 求1~x的和
    return x * (x + 1) / 2;
}
int main() {
    
    int n, x, y;            // 设总数 x，“我家”门牌号 y
    scanf("%d", &n);
    for(x = 1; ; x ++) {    // 枚举 x 的情况
        int sum = Sum(x);
        // 1~x的和减去3倍的y，等于其他家门牌号的和减去2倍的y
        // 反之，1~x的和减去 n，等于 3倍的 y，如果结果能被 3 整除则找到答案
        if(sum > n && (sum - n) % 3 == 0) {
            y = (sum - n) / 3;
            break;
        }
    }
    printf("%d %d\n", y, x);
    return 0;
}
```