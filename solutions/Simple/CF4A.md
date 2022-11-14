### `Watermelon`

> difficulty: 1

多年没碰算法题，最近打算重新拾起，先尝试个水题恢复下自信，没想到还WA了一次，因为我竟然把 even 脑补成了奇数……

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){
    int w;
    while(scanf("%d", &w) != EOF) {
        printf((w % 2) || w == 2 ? "NO\n":"YES\n");
    }
    return 0;
}


```

