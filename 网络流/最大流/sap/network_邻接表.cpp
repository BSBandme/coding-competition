/*
 *  call ::
 *  pre(n, source, terminal);
 *  for(int i = 0; i < m; i++) {
 *  	addedge(from, to, capcity);
 *  }
 *  SAP();
 *
 *  返回maxflow;
 */
#define MAXM 10000
#define MAXN 300
#define Typec ll
#define Clr(x) memset(x,0,sizeof(x))
#define INF 0x5fffffffffffLL

struct sap {
	struct edge {
		int to, next;
		Typec cap;
	} e[MAXM * 2];

	int p, head[MAXN], h[MAXN], vh[MAXN];
	int n, m, s, t;
	Typec maxFlow;

	inline void addedge(int a, int b, Typec c) {
		e[p].to = b, e[p].cap = c, e[p].next = head[a], head[a] = p++;
		e[p].to = a, e[p].cap = 0, e[p].next = head[b], head[b] = p++;
	}
	Typec dfs(int u, Typec pre) {
		if (u == t)
			return pre;
		Typec now, aug, tmp = pre;
		int mh = n + 10, to;
		for (now = head[u]; now; now = e[now].next) {
			to = e[now].to;
			if (e[now].cap) {
				if (pre && h[u] == h[to] + 1) {
					aug = dfs(to, min(pre, e[now].cap));
					pre -= aug;
					e[now].cap -= aug;
					e[now ^ 1].cap += aug;
					if (h[s] >= n)
						return tmp - pre;
				}
				mh = min(mh, h[to]);
			}
		}
		if (tmp - pre > 0)
			return tmp - pre;
		--vh[h[u]];
		if (!vh[h[u]]) {
			h[s] = n;
			return 0;
		}
		++vh[h[u] = mh + 1];
		return 0;
	}

	vector<int> FindMinCut() {
		vector<int> ans;
		int q[MAXN], in[MAXN];
		memset(in, 0, sizeof(in));
		int lq = 0;
		in[s] = 1;
		q[lq++] = s;
		for (int i = 0; i < lq; i++) {
			int no = q[i];
			for (int re = head[no]; re; re = e[re].next) if (e[re].cap && !in[e[re].to]) {
				in[e[re].to] = 1;
				q[lq++] = e[re].to;
			}
		}
		for (int i = 0; i < lq; i++) {
			int no = q[i];
			for (int re = head[no]; re; re = e[re].next) if (!in[e[re].to]) {
				ans.push_back(re);
			}
		}
		return ans;
	}

	void init() {
		maxFlow = 0;
		Clr(h);
		Clr(vh);
		vh[0] = n;
	}
	void SAP() {
		init();
		while (h[s] < n)
			maxFlow += dfs(s, INF);
	}
	void pre(int nn, int ss = -1, int tt = -1) {
		p = 2;
		Clr(head);
		n = nn;
		if(ss == -1) s = 0;
		else s = ss;
		if (tt == -1) t = n - 1;
		else t = tt;
	}
} g;