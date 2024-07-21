### `素数个数`

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
int main() {
    int n, cnt;
    scanf("%d", &n);
    cnt = 0;
    for(int i = 2; i <= n; i ++) {
        cnt += IsPrime(i);
    }
    printf("%d\n", cnt);
    return 0;
}
```