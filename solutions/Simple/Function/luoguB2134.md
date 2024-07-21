### `质数的和与积`

> difficulty: 1

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
bool IsPrime(int x) {
    for(int i = 2; i * i <= x; i ++) {
        if(x % i == 0) {
            return false;
        }
    }
    return true;
}
double Max2(double a, double b) {
    return a > b ? a : b;
}
int main() {
    int S, ans;
    scanf("%d", &S);
    ans = 0;
    for(int i = 2; i * 2 <= S; i ++) {
        if(IsPrime(i) && IsPrime(S - i)) {
            ans = Max2(ans, i * (S - i));
        }
    }
    printf("%d\n", ans);
    return 0;
}
```