// Same Parity Summands
// 这题比1355D容易一些，根据奇偶性凑 1 或 2 就可以了，没什么要证明的
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


int main(){
    int t;
    scanf("%d", &t);
    while(t --) {
        int n, k;
        scanf("%d%d", &n, &k);
        if(n % 2 && !(k % 2) || n < k || !(n % 2) && k % 2 && n < 2 * k) {
            printf("NO\n");
        }
        else {
            printf("YES\n");
            int num = !(n % 2) && k % 2 ? 2 : 1;
            for(int i = 0; i < k - 1; i ++){
                printf("%d ", num);
            }
            printf("%d\n", n - num * (k - 1));
        }
    }
    return 0;
}
