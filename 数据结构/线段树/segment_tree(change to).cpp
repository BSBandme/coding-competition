
//用于更新方式为将一段改为某个数
template <class t> struct segment_node{
	int be, en;
	t newnum, sum, jie, time, rt;
	bool change;
};
template <class t> struct segment_tree{
	int l;
	segment_node <t> tree[maxn * 4];
	
	inline int gleft(int no) {return no << 1;}
	inline int gright(int no) {return (no << 1) + 1;}
	inline int gfa(int no) {return no >> 1;}
	inline segment_tree(){ l = 0; }
	
	void build(int no, int l, int r, t jie, t orig = 0, t *a = NULL){
		if(l == r){
			tree[no].be = tree[no].en = l;
			tree[no].jie = jie;
			if(a == NULL) tree[no].sum = orig;
			else tree[no].sum = a[l];
			tree[no].newnum = 0;
			tree[no].rt = tree[no].time = -1;
			tree[no].change = 0;
			return;
		}
		tree[no].be = l; tree[no].en = r;
		int mid = (l + r) / 2;
		build(gleft(no), l, mid, jie, orig, a);
		build(gright(no), mid + 1, r, jie, orig, a);
		tree[no].sum = (tree[gleft(no)].sum + tree[gright(no)].sum) % mod ;
		tree[no].newnum = 0;
		tree[no].rt = tree[no].time = -1;
		tree[no].jie = jie;
		tree[no].change = 0;
	}
	//l到r所有节点加ranadd
	void down(int l, int r, int no, t ranadd, t time){
		if(l <= tree[no].be && r >= tree[no].en){
			tree[no].newnum = ranadd;
			tree[no].sum = (ranadd * ((arrche[tree[no].jie][tree[no].en] - arrche[tree[no].jie][tree[no].be - 1] + mod) % mod)) % mod;
			tree[no].change = 1;
			tree[no].rt = tree[no].time = time;
			return;
		}
		int mid = (tree[no].be + tree[no].en) / 2;
		if(r >= tree[no].be && l <= mid) {
			if(tree[gleft(no)].rt < tree[no].time) down(tree[gleft(no)].be, tree[gleft(no)].en, gleft(no), tree[no].newnum, tree[no].time);
			tree[no].sum -= tree[gleft(no)].sum;
			down(l, r, gleft(no), ranadd, time);
			tree[no].sum += tree[gleft(no)].sum;
		}
		if(r >= mid + 1 && l <= tree[no].en){
			if(tree[gright(no)].rt < tree[no].time) down(tree[gright(no)].be, tree[gright(no)].en, gright(no), tree[no].newnum, tree[no].time);
			tree[no].sum -= tree[gright(no)].sum;
			down(l, r, gright(no), ranadd, time);
			tree[no].sum += tree[gright(no)].sum;
		}
		tree[no].sum += 2 * mod;
		tree[no].sum %= mod;
		tree[no].change = 1;
		tree[no].rt = time;
	}
	//l到r所以节点求和
	t getsum(int l, int r, int no, ll newnum, ll time){
		if(l > r) return 0;
		ll newnum1 = newnum, time1 = time;
		if(time1 < tree[no].time) {
			time1 = tree[no].time;
			newnum1 = tree[no].newnum;
		}
		if(l <= tree[no].be && r >= tree[no].en){
			if(time1 > tree[no].rt){
				if(time1 == -1) return tree[no].sum;
				else return (newnum1 * ((arrche[tree[no].jie][tree[no].en] - arrche[tree[no].jie][tree[no].be - 1] + mod) % mod)) % mod;
			}else return tree[no].sum;
		}
		t ans = 0;
		int mid = (tree[no].be + tree[no].en) / 2;
		
		if(r >= tree[no].be && l <= mid) ans += getsum(max1(l, tree[no].be), min1(r, mid), gleft(no), newnum1, time1);
		if(r >= mid + 1 && l <= tree[no].en) ans += getsum(max1(l, mid + 1), min1(r, tree[no].en), gright(no), newnum1, time1);
		return ans % mod;
	}
};
