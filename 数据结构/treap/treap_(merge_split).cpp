namespace __treap {

const int SEED = 1537;
const int MAXTN = 300000;

int nextRandom() {
	static int result = 1;
	result = ((result | 1) * SEED) ^ 123;

	return result;
}
struct TreapNode {
	TreapNode *l, *r, *fa;
	int key, weight;
	ll sz, leftv, rightv, v, addv, multiv, minv;
	ll sum;
	TreapNode() {
	}

	void Reset(int v) {
		this->sz = 1;
		this->fa = this->l = this->r = nullptr;
		this->v = this->leftv = this->rightv = v;
		this->minv = this->sum = v;
		this->multiv = this->addv = 0;
		this->weight = nextRandom();
	}

	static int GetSize(TreapNode *x) {
    if (nullptr == x) {
      return 0;
    }
    return x->sz;
  }

	void Upd(ll raddv, ll rmultiv) {
		if (l != nullptr) {
			v += raddv + rmultiv * l->sz;
			leftv += raddv;
		} else {
			v += raddv;
			leftv = v;
		}
		if (r != nullptr) {
			rightv += raddv + (sz - 1) * rmultiv;
		} else {
			rightv = v;
		}
		addv += raddv;
		multiv += rmultiv;
	}

	void PushDown() {
		if (addv == 0 && multiv == 0) return;
		if (l != nullptr) l->Upd(addv, multiv);
		if (r != nullptr) r->Upd(addv + multiv * (GetSize((l)) + 1), multiv);
		addv = multiv = 0;
	}

	void PushUp() {
		sz = 1;
		leftv = rightv = minv = v;
		sum = v;


		if (l != nullptr) {
			sz += l->sz;
			sum += l->sum;
			minv = min(minv, l->minv);
			leftv = l->leftv;
		}

		if (r != nullptr) {
			sz += r->sz;
			sum += r->sum;
			minv = min(minv, r->minv);
			rightv = r->rightv;
		}
	}

  TreapNode* Nxt() {
    TreapNode* ans = this;
    if (ans->r == nullptr) {
      auto* rans = ans->fa;
      while (rans && ans == rans->r) {
        ans = rans;
        rans = rans->fa;
      }
      ans = rans;
    } else {
      ans = ans->r;
      while (ans->l) ans = ans->l;
    }
    return ans;
  }

  TreapNode* Bef() {
    TreapNode* ans = this;
    if (ans->r == nullptr) {
      auto* rans = ans->fa;
      while (rans && ans == rans->r) {
        ans = rans;
        rans = rans->fa;
      }
      ans = rans;
    } else {
      ans = ans->r;
      while (ans->l) ans = ans->l;
    }
    return ans;
  }

  TreapNode* Begin() {
    TreapNode* ans = this;
    while (ans->l) ans = ans->l;
    return ans;
  }

  TreapNode* Back() {
    TreapNode* ans = this;
    while (ans->r) ans = ans->r;
    return ans;
  }

} treapNode[MAXTN];

int GetSize(TreapNode *x) {
	if (nullptr == x) {
		return 0;
	}
	return x->sz;
}

int GetMinv(TreapNode *x) {
	if (nullptr == x) {
		return 1 << 30;
	}
	return x->minv;
}

ll GetSum(TreapNode *x) {
	if (nullptr == x) {
		return 1 << 30;
	}
	return x->sum;
}

//初始化注意!
int ptr = 0;
TreapNode* GetNode(int h) {
	treapNode[ptr].Reset(h);
	return &treapNode[ptr++];
}

//合并x与y，返回根
TreapNode* Merge(TreapNode *x, TreapNode *y) {
	if (x == nullptr) {
		return y;
	}
	if (y == nullptr) {
		return x;
	}
	if (x->weight < y->weight) {
		x->PushDown();
		x->r = Merge(x->r, y);
		x->PushUp();

		return x;
	} else {
		y->PushDown();
		y->l = Merge(x, y->l);
		y->PushUp();

		return y;
	}
}

//L放前K大(K = 1, 2, ...)
void Split(TreapNode *p, TreapNode *&x, TreapNode *&y, int k) {
	if (k == 0) {
		x = nullptr;
		y = p;
	} else if (k == GetSize(p)) {
		x = p;
		y = nullptr;
	} else {
		p->PushDown();
		if (GetSize(p->l) >= k) {
			y = p;
			Split(p->l, x, y->l, k);
			y->PushUp();
		} else {
			x = p;
			Split(p->r, x->r, y, k - GetSize(p->l) - 1);
			x->PushUp();
		}
	}
}

//L放前K大(K = 1, 2, ...)
void SplitToLastBelowK(TreapNode *p, TreapNode *&x, TreapNode *&y, int k) {
	if (GetMinv(p) >= k) {
		x = nullptr;
		y = p;
	} else {
		p->PushDown();
		if (GetMinv(p->r) < k || p->v < k) {
			x = p;
			SplitToLastBelowK(p->r, x->r, y, k);
			x->PushUp();
		} else {
			y = p;
			SplitToLastBelowK(p->l, x, y->l, k);
			y->PushUp();
		}
	}
}

//L放前K大(K = 1, 2, ...)
TreapNode* GetKth(TreapNode *p, int k) {
	if (k == 0) {
		return nullptr;
	} else if (k > GetSize(p)) {
		return nullptr;
	} else {
		p->PushDown();
		if (GetSize(p->l) >= k) {
			return GetKth(p->l, k);
		} else if (GetSize(p->l) + 1 == k) {
			return p;
		} else {
			return GetKth(p->r, k - GetSize(p->l) - 1);
		}
	}
}

void Upd(TreapNode *p, int k, ll addv, ll multiv) {
	if (k == 0) {
		return;
	} else if (k >= TreapNode::GetSize(p)) {
		return p->Upd(addv, multiv);
	} else {
		p->PushDown();
		if (p->l) {
			Upd(p->l, k, addv, multiv);
		}
		if (TreapNode::GetSize(p->l) + 1 <= k) {
			p->v += addv + multiv * TreapNode::GetSize(p->l);
		}
		if (k -= TreapNode::GetSize(p->l) + 1; k > 0) {
			addv += multiv * (TreapNode::GetSize(p->l) + 1);
			Upd(p->r, k, addv, multiv);
		}
		p->PushUp();
	}
}

}  // namespace __treap;

using namespace __treap;
