// difficulty: 3
// 双向链表练习题
// 维护正反两个方向的链表，就可以避免模拟翻转了。
// stl的 list 可以用 splice 拼接链表。在处理正反逻辑时要注意一个地方：
// b链表与a链表交换用 swap 很快，但如果 a = b; b.clear(); 就是深拷贝了，会超时。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<list>
const int maxn = 1e5 + 10;
int n, m;
std::list<int> lt[2][maxn];
int ldir[maxn];
int main()
{
	int a, b;
	while(scanf("%d%d", &n, &m) != EOF)
	{
		for(int i = 1; i <= n; i ++)
		{
			for(int j = 0; j < 2; j ++)
			{
				lt[j][i].clear();
				lt[j][i].push_back(i);
			}
			ldir[i] = 0;
		}
		for(int i = 0; i < m; i ++)
		{
			scanf("%d%d", &a, &b);
			lt[ldir[a]][a].splice(lt[ldir[a]][a].end(), lt[ldir[b]][b]);
			lt[!ldir[b]][b].splice(lt[!ldir[b]][b].end(), lt[!ldir[a]][a]);
			std::swap(lt[!ldir[a]][a], lt[!ldir[b]][b]);
			ldir[a] = !ldir[a];			
		}
		printf("%d", (int)lt[ldir[1]][1].size());
		for(auto i : lt[ldir[1]][1])
			printf(" %d", i);
		printf("\n");
	}
	return 0;
}