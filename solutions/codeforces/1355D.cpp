// 虽然看了答案构造 n-1 个 1 ，剩余数量大于 n 很简单。
// 但不知道怎么证明不满足这个情况的时候一定无解，数学还是怕。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){
    int n, s;
    while(scanf("%d%d", &n, &s) != EOF) {
        if(s - n + 1 > n) {
            printf("YES\n");
            for(int i = 0; i < n - 1; i ++){
                printf("1 ");
            }
            printf("%d\n", s - n + 1);
            printf("%d\n", n);
        }
        else {
            printf("NO\n");
        }
    }
    return 0;
}
