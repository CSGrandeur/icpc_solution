// difficulty: 2
// Numbers
// 直接回溯枚举，一位数最多10个，且一位数一旦确定，整个数组划分即被确定。
// 总情况数至多为一位数选择数，假设 i 的个数为 ai，i为[0, 9]，那么选择方案数为 Πai <= (50/10) ^ 10 ≈ 1e7，
// 加上剪枝远不到该复杂度。
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char buf[55];
int ans;
bool vis[100];
void DFS(int now, int last)
{
	if(!buf[now]) {ans += last == -1; return;}
	if(last != -1)
	{
		if(vis[last + buf[now] - '0']) return;
		vis[last + buf[now] - '0'] = true;
		DFS(now + 1, -1);
		vis[last + buf[now] - '0'] = false;
	}
	else
	{
		if(buf[now] != '0')
			DFS(now + 1, (buf[now] - '0') * 10);
		if(!vis[buf[now] - '0'])
		{
			vis[buf[now] - '0'] = true;
			DFS(now + 1, -1);
			vis[buf[now] - '0'] = false;
		}
	}
}
int main()
{
	memset(vis, 0, sizeof(vis));
	while(scanf("%s", buf) != EOF)
	{
		ans = 0;
		DFS(0, -1);
		printf("%d\n", ans);
	}
	return 0;
}
