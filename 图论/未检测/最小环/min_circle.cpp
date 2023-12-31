#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const int maxn = 300;
const int inf = 2000000000;
int map[maxn][maxn];
int n, m;

int main()
{
	int i, j, k;
	int ans = inf;

	freopen("min_circle.in", "r", stdin);
	scanf("%d", &n, &m);
	for(int i1 = 0; i1 < m; i1++)
	{
		scanf("%d%d%d", &i, &j, &k);
		i--; j--;
		map[i][j] = k;
		map[j][i] = k;
	}

	memset(map, -1, sizeof(map));
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < i - 1; j++)
			for(k = 0; k < i - 1; k++)			
				if(map[i][j] != -1 && map[i][k] != -1)
					ans = ans < map[i][j] + map[i][k] + map[j][k] ? ans : map[i][j] + map[i][k] + map[j][k];
		for(j = 0; j < n; j++)
			for(k = 0; k < n; k++)
				if(map[i][j] != -1 && map[i][k] != -1)
					if(map[j][k] == -1 || map[j][k] < map[i][j] + map[i][k])
						map[j][k] = map[i][j] + map[i][k];
	}

	cout << ans << endl;
			
	return 0;
}