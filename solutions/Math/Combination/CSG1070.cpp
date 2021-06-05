// difficulty: 3
// 组合数
// 比赛中很多人 128位int，甚至 long double 暴力水过去
// 优化一点的正解，所有组合数的 K 大到一定程度（几十）就一定超过1e18了
// 对k为0、1、2的情况特判输出，其余的 k 能打出一个可以承受的表，用vector动态伸缩更省空间
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
#include<vector>
using namespace std;
const long long maxres = 1e18 + 1e-8;
int n, k;
int endi;
vector<vector<long long> >cm;

void Init()
{
	cm.push_back(vector<long long>(1));
	cm.push_back(vector<long long>(2));
	cm[0][0] = cm[1][0] = cm[1][1] = 1;
	for(endi = 2; ; endi ++)
	{
		bool findFlag = false;
		cm.push_back(vector<long long>(3));
		cm[endi][0] = 1;
		cm[endi][1] = endi;
		cm[endi][2] = (long long)endi * (endi - 1) >> 1;
		for(int i = 3; i <= (endi >> 1); i ++)
		{
			cm[endi].push_back(cm[endi - 1][i - 1] + cm[endi - 1][min(i, endi - 1 - i)]);
			if(cm[endi][i] > maxres) break;
			findFlag = true;
		}
		if(!findFlag && endi > 6) break;
	}
}
long long Solve()
{
	k = std::min(k, n - k);
	if(k >= 3 && n >= endi) return maxres;
	if(k == 0) return 1;
	if(k == 1) return n;
	if(k == 2) return std::min((long long)n * (n - 1) >> 1, maxres);
	if(k >= cm[n].size()) return maxres;
	return cm[n][k] > maxres ? maxres : cm[n][k];
}
int main()
{
	Init();
	while(scanf("%d%d", &n, &k) != EOF)
		printf("%lld\n", Solve());
	return 0;
}