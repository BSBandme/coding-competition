#ifndef SEGMENT_TREE_MAXN
#define SEGMENT_TREE_MAXN 200100
#define NODE_VAL_TYPE int
#endif
struct SegmentNode{
	int be, en;
	NODE_VAL_TYPE maxv, add, sum;
};
struct SegmentTree{
	int l;
	SegmentNode tree[SEGMENT_TREE_MAXN * 4];

	inline int GetLeft(int no) {return no << 1;}
	inline int GetRight(int no) {return (no << 1) + 1;}
	inline int GetFa(int no) {return no >> 1;}
	inline SegmentTree(){ l = 0; }

	void Build(int no, int l, int r, NODE_VAL_TYPE orig = 0, NODE_VAL_TYPE *a = NULL){
		if(l > r) r = l;
		if(l == r){
			tree[no].be = tree[no].en = l;
			tree[no].add = 0;
			if(a == NULL) tree[no].maxv = orig;
			else tree[no].maxv = a[l];
			tree[no].add = 0;
			tree[no].sum = tree[no].maxv;
			return;
		}
		tree[no].be = l; tree[no].en = r;
		int mid = (l + r) / 2;
		Build(GetLeft(no), l, mid, orig, a);
		Build(GetRight(no), mid + 1, r, orig, a);
		tree[no].add = 0;
		PushUp(no);
	}
	inline void PushUp(int no) {
		tree[no].sum = tree[GetLeft(no)].sum + tree[GetRight(no)].sum;
		tree[no].maxv = max1(tree[GetLeft(no)].maxv, tree[GetRight(no)].maxv);
	}
	inline void PushDown(int no) {
		int le = GetLeft(no), ri = GetRight(no);
		tree[le].add += tree[no].add;
		tree[le].sum += tree[no].add * (tree[le].en - tree[le].be + 1);
		tree[le].maxv += tree[no].add;
		tree[ri].add += tree[no].add;
		tree[ri].sum += tree[no].add * (tree[ri].en - tree[ri].be + 1);
		tree[ri].maxv += tree[no].add;
		tree[no].add = 0;
	}
	void Upd(int l, int r, int no, NODE_VAL_TYPE ranadd){
		if(l <= tree[no].be && r >= tree[no].en){
			tree[no].add += ranadd;
			tree[no].sum += ranadd * (tree[no].en - tree[no].be + 1);
			tree[no].maxv += ranadd;
			return;
		}
		if(tree[no].add && tree[no].be != tree[no].en) PushDown(no);
		int mid = (tree[no].be + tree[no].en) >> 1;
		if(r >= tree[no].be && l <= mid) Upd(l, r, GetLeft(no), ranadd);
		if(r >= mid + 1 && l <= tree[no].en) Upd(l, r, GetRight(no), ranadd);
		PushUp(no);
	}
	NODE_VAL_TYPE GetSum(int l, int r, int no){
		if(l > r) return 0;
		if(l <= tree[no].be && r >= tree[no].en)
			return tree[no].sum;
		if(tree[no].add && tree[no].be != tree[no].en) PushDown(no);
		NODE_VAL_TYPE ans = 0;
		int mid = (tree[no].be + tree[no].en) >> 1;
		if(r >= tree[no].be && l <= mid) ans += GetSum(l, r, GetLeft(no));
		if(r >= mid + 1 && l <= tree[no].en) ans += GetSum(l, r, GetRight(no));
		return ans;
	}
	int GetRank(int rank, int no){
		if(rank > tree[no].sum) return -1;
		if(tree[no].be == tree[no].en) return tree[no].be;
		if(tree[no].add && tree[no].be != tree[no].en) PushDown(no);
		int mid = (tree[no].be + tree[no].en) >> 1;
		if(rank <= tree[GetLeft(no)].sum) return GetRank(rank, GetLeft(no));
		return GetRank(rank - tree[GetLeft(no)].sum, GetRight(no));
	}
	inline int GetSectionRank(int l, int r, int rank){
		return GetRank(rank + GetSum(0, l - 1, 1), 1);
	}
};
