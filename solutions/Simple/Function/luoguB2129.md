### `最大数 max(x,y,z)`

> difficulty: 1

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
double Max2(double a, double b) {
    return a > b ? a : b;
}
double Max3(double a, double b, double c) {
    return Max2(Max2(a, b), c);
}
int main() {
    
    double a, b, c;
    scanf("%lf%lf%lf", &a, &b, &c);
    printf("%.3f\n", Max3(a, b, c) / (Max3(a + b, b, c) * Max3(a, b, b + c)));
    return 0;
}
```