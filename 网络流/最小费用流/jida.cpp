/*===================================================================*\
 | 最小费用流 O(V * E * f)
 | INIT: network g; 
 |		 g.build(v); 
 |		 for(int i = 0; i < e; e++) 
 |		 	addedge(a, b, capcity, cost);
 | CALL: g.mincost(s, t); 
 |       flow=g.flow; 
 |       cost=g.cost;
 | 注意: SPFA增广, 实际复杂度远远小于O(V * E);
 \*===================================================================*/
#define typef int // type of flow
#define typec int // type of dis
const typef MINC_INFF = 0x3f3f3f3f; // max of flow
const typec MINC_INFC = 0x3f3f3f3f; // max of dis
const int MINC_MAXE = MAXE;
const int MINC_MAXN = MAXN;
struct network {
  int nv, ne, pnt[MINC_MAXE], nxt[MINC_MAXE];
  int vis[MINC_MAXN], que[MINC_MAXN], head[MINC_MAXN], pv[MINC_MAXN], pe[MINC_MAXN];
  typef flow, cap[MINC_MAXE];
  typec cost, dis[MINC_MAXE], d[MINC_MAXN];
  void addedge(int u, int v, typef c, typec w) {
    pnt[ne] = v;
    cap[ne] = c;
    dis[ne] = +w;
    nxt[ne] = head[u];
    head[u] = (ne++);
    pnt[ne] = u;
    cap[ne] = 0;
    dis[ne] = -w;
    nxt[ne] = head[v];
    head[v] = (ne++);
  }
  typec mincost(int src, int sink) {
    int i, k, f, r;
    typef mxf;
    for (flow = 0, cost = 0;;) {
      memset(pv, -1, sizeof(pv));
      memset(vis, 0, sizeof(vis));
      for (i = 0; i < nv; ++i)
        d[i] = MINC_INFC;
      d[src] = 0;
      pv[src] = src;
      vis[src] = 1;
      for (f = 0, r = 1, que[0] = src; r != f;) {
        i = que[f++];
        vis[i] = 0;
        if (MINC_MAXN == f)
          f = 0;
        for (k = head[i]; k != -1; k = nxt[k])
          if (cap[k] && dis[k] + d[i] < d[pnt[k]]) {
            d[pnt[k]] = dis[k] + d[i];
            if (0 == vis[pnt[k]]) {
              vis[pnt[k]] = 1;
              que[r++] = pnt[k];
              if (MINC_MAXN == r)
                r = 0;
            }
            pv[pnt[k]] = i;
            pe[pnt[k]] = k;
          }
      }
      if (-1 == pv[sink])
        break;
      for (k = sink, mxf = MINC_INFF; k != src; k = pv[k])
        if (cap[pe[k]] < mxf)
          mxf = cap[pe[k]];
      flow += mxf;
      cost += d[sink] * mxf;
      for (k = sink; k != src; k = pv[k]) {
        cap[pe[k]] -= mxf;
        cap[pe[k] ^ 1] += mxf;
      }
    }
    return cost;
  }
  void init(int v) {
    nv = v;
    ne = 0;
    memset(head, -1, sizeof(head));
  }
} g;

