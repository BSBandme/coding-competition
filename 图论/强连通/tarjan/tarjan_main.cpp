/*==================================================*\
| Tarjan强连通分量
| INIT: vec[]为邻接表; stop, cnt, scnt置0; pre[]置-1;
| CALL: for(i=0; i<n; ++i) if(-1==pre[i]) tarjan(i, n);
\*==================================================*/
const int V = 200000;
const int maxm = 400000;
struct edge {
	int to, nxt;
} e[maxm];
int head[maxm], le;
inline void addedge(int a, int b) {
	e[le].to = b;
	e[le].nxt = head[a];
	head[a] = le++;
}
int id[V], pre[V], low[V], s[V], stop, cnt, scnt;
void tarjan(int v, int n) { // vertex: 0 ~ n-1
	int t, minc = low[v] = pre[v] = cnt++;
	s[stop++] = v;
	for (int i = head[v]; i != -1; i = e[i].nxt) {
		if (-1 == pre[e[i].to]) tarjan(e[i].to, n);
		if (low[e[i].to] < minc) minc = low[e[i].to];
	}
	if (minc < low[v]) {
		low[v] = minc;
		return;
	}
	do {
		id[t = s[--stop]] = scnt;
		low[t] = n;
	} while (t != v);
	++scnt; // 强连通分量的个数
}