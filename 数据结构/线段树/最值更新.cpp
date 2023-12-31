#ifndef SEGMENT_TREE_MAXN
#define SEGMENT_TREE_MAXN 150100
#define NODE_VAL_TYPE ll
#endif
struct SegmentNode{
	int be, en;
	NODE_VAL_TYPE peak, sec_peak, sum, cnt_peak, cnt;
	NODE_VAL_TYPE lazy_add;
};
class SegmentTree{
 public:
	int l;
	NODE_VAL_TYPE inf;
	SegmentNode tree[SEGMENT_TREE_MAXN * 4];
	function<bool(const NODE_VAL_TYPE&, const NODE_VAL_TYPE&)> comp;

	inline int GetLeft(int no) {return no << 1;}
	inline int GetRight(int no) {return (no << 1) + 1;}
	inline int GetFa(int no) {return no >> 1;}
	SegmentTree(function<bool(const NODE_VAL_TYPE&, const NODE_VAL_TYPE&)> func) {
		l = 0;
		comp = func;
		if (comp(std::numeric_limits<NODE_VAL_TYPE>::min(),
				std::numeric_limits<NODE_VAL_TYPE>::max())) {
			inf = std::numeric_limits<NODE_VAL_TYPE>::max();
		} else {
			inf = std::numeric_limits<NODE_VAL_TYPE>::min();
		}
	}

	void Build(int no, int l, int r){
		if(l > r) r = l;
		if(l == r){
			tree[no].be = tree[no].en = l;
			tree[no].lazy_add = 0;
			tree[no].sum = tree[no].cnt = 0;
			tree[no].peak = tree[no].sec_peak = inf;
			tree[no].cnt_peak = 0;
			return;
		}
		tree[no].be = l; tree[no].en = r;
		int mid = (l + r) / 2;
		Build(GetLeft(no), l, mid);
		Build(GetRight(no), mid + 1, r);
		tree[no].lazy_add = 0;
		PushUp(no);
	}
	inline void PushUp(int no) {
		int le = GetLeft(no), ri = GetRight(no);
		tree[no].sum = tree[le].sum + tree[ri].sum;
		tree[no].cnt = tree[le].cnt + tree[ri].cnt;
		if (comp(tree[le].peak, tree[ri].peak)) {
			tree[no].peak = tree[le].peak;
			tree[no].cnt_peak = tree[le].cnt_peak;
			tree[no].sec_peak =
					comp(tree[le].sec_peak, tree[ri].peak) ?
							tree[le].sec_peak : tree[ri].peak;
		} else if (comp(tree[ri].peak, tree[le].peak)) {
			tree[no].peak = tree[ri].peak;
			tree[no].cnt_peak = tree[ri].cnt_peak;
			tree[no].sec_peak =
					comp(tree[ri].sec_peak, tree[le].peak) ?
							tree[ri].sec_peak : tree[le].peak;
		} else {
			tree[no].peak = tree[le].peak;
			tree[no].cnt_peak = tree[le].cnt_peak + tree[ri].cnt_peak;
			tree[no].sec_peak =
					comp(tree[ri].sec_peak, tree[le].sec_peak) ?
							tree[ri].sec_peak : tree[le].sec_peak;
		}
		tree[no].lazy_add = 0;
	}
	inline void PushDown(int no) {
		int le = GetLeft(no), ri = GetRight(no);
		if (tree[no].lazy_add) {
			tree[le].lazy_add += tree[no].lazy_add;
			tree[le].sum += tree[no].lazy_add * tree[le].cnt;
			if (tree[le].peak != inf) tree[le].peak += tree[no].lazy_add;
			if (tree[le].sec_peak != inf)tree[le].sec_peak += tree[no].lazy_add;
			tree[ri].lazy_add += tree[no].lazy_add;
			tree[ri].sum += tree[no].lazy_add * tree[ri].cnt;
			if (tree[ri].peak != inf) tree[ri].peak += tree[no].lazy_add;
			if (tree[ri].sec_peak != inf) tree[ri].sec_peak += tree[no].lazy_add;
		}
		if (comp(tree[le].peak, tree[no].peak)) {
			tree[le].sum -= tree[le].cnt_peak * (tree[le].peak - tree[no].peak);
			tree[le].peak = tree[no].peak;
		}
		if (comp(tree[ri].peak, tree[no].peak)) {
			tree[ri].sum -= tree[ri].cnt_peak * (tree[ri].peak - tree[no].peak);
			tree[ri].peak = tree[no].peak;
		}
		tree[no].lazy_add = 0;
	}
	void UpdTo(int l, int r, int no, NODE_VAL_TYPE bound ){
		if (l > r) return;
		if(l <= tree[no].be && r >= tree[no].en && !comp(tree[no].sec_peak, bound)) {
			if (comp(tree[no].peak, bound)) {
				tree[no].sum -= tree[no].cnt_peak * (tree[no].peak - bound);
				tree[no].peak = bound;
			}
			return;
		}
		if (tree[no].be != tree[no].en) PushDown(no);
		int mid = (tree[no].be + tree[no].en) >> 1;
		if (r >= tree[no].be && l <= mid) UpdTo(l, r, GetLeft(no), bound);
		if (r >= mid + 1 && l <= tree[no].en) UpdTo(l, r, GetRight(no), bound);
		PushUp(no);
	}
	void UpdAdd(int l, int r, int no, NODE_VAL_TYPE ranadd) {
		if (l > r) return;
		if (l <= tree[no].be && r >= tree[no].en){
			tree[no].lazy_add += ranadd;
			tree[no].sum += ranadd * tree[no].cnt;
			if (comp(tree[no].peak, inf)) tree[no].peak += ranadd;
			if (comp(tree[no].sec_peak, inf)) tree[no].sec_peak += ranadd;
			return;
		}
		if (tree[no].be != tree[no].en) PushDown(no);
		int mid = (tree[no].be + tree[no].en) >> 1;
		if (r >= tree[no].be && l <= mid) UpdAdd(l, r, GetLeft(no), ranadd);
		if (r >= mid + 1 && l <= tree[no].en) UpdAdd(l, r, GetRight(no), ranadd);
		PushUp(no);
	}
	void AddNewVal(int loc, int no, NODE_VAL_TYPE new_val) {
		if (loc <= tree[no].be && loc >= tree[no].en) {
			tree[no].cnt = 1;
			tree[no].sum = new_val;
			tree[no].peak = new_val;
			tree[no].cnt_peak = 1;
			tree[no].lazy_add = 0;
			return;
		}
		if (tree[no].be != tree[no].en) PushDown(no);
		int mid = (tree[no].be + tree[no].en) >> 1;
		if (loc <= mid) AddNewVal(loc, GetLeft(no), new_val);
		else AddNewVal(loc, GetRight(no), new_val);
		PushUp(no);
	}
	NODE_VAL_TYPE GetSum(int l, int r, int no){
		if (l > r) return 0;
		if (l <= tree[no].be && r >= tree[no].en)
			return tree[no].sum;
		if (tree[no].be != tree[no].en) PushDown(no);
		NODE_VAL_TYPE ans = 0;
		int mid = (tree[no].be + tree[no].en) >> 1;
		if (r >= tree[no].be && l <= mid) ans += GetSum(l, r, GetLeft(no));
		if (r >= mid + 1 && l <= tree[no].en) ans += GetSum(l, r, GetRight(no));
		return ans;
	}
	int GetRank(int rank, int no){
		if (rank > tree[no].sum) return -1;
		if (tree[no].be == tree[no].en) return tree[no].be;
		if (tree[no].be != tree[no].en) PushDown(no);
		if (rank <= tree[GetLeft(no)].sum) return GetRank(rank, GetLeft(no));
		return GetRank(rank - tree[GetLeft(no)].sum, GetRight(no));
	}
	inline int GetSectionRank(int l, int r, int rank){
		return GetRank(rank + GetSum(0, l - 1, 1), 1);
	}
};