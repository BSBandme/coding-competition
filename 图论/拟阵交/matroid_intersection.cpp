class Matroid {
 public:
  using UI = unordered_set<int>;
  Matroid(int n) : n_(n) {}
  virtual unordered_map<int, vi> GetAllCanExchange(
      UI* current_set, bool rev_edge) {
    unordered_map<int, vi> ans;
    for (int i = 0; i < n_; i++) {
      if (current_set->find(i) == current_set->end()) {
        current_set->insert(i);
        for (const auto it : *current_set) if (it != i) {
          current_set->erase(i);
          if (Oracle(current_set)) {
            if (rev_edge) ans[i].push_back(it);
            else ans[it].push_back(i);
          }
          current_set->insert(i);
        }
      }
    }
    return ans;
  }
  virtual UI GetAllCanExtend(UI* current_set) {
    UI ans;
    for (int i = 0; i < n_; i++) {
      if (current_set->find(i) == current_set->end()) {
        current_set->insert(i);
        if (Oracle(current_set)) {
          ans.insert(i);
        }
        current_set->erase(i);
      }
    }
    return ans;
  }
  virtual bool Oracle(UI* ui) = 0;

 protected:
  int n_;
};

class MatroidIntersection {
 public:
  using UI = unordered_set<int>;
  MatroidIntersection(int n, Matroid* a, Matroid* b) :
      n_(n), weight_(vi{}), a_(a), b_(b),
      Augment(&MatroidIntersection::AugmentNormal) {}
  MatroidIntersection(const vi& weight, Matroid* a, Matroid* b) :
      n_(weight.size()), weight_(weight), a_(a), b_(b),
      Augment(&MatroidIntersection::AugmentWeighted) {}

  UI GetMaxIntersection() {
    UI ans;
    while ((this->*Augment)(&ans));
    return ans;
  }

 private:
  bool AugmentNormal(UI* current_set) {
    UI y1 = a_->GetAllCanExtend(current_set);
    UI y2 = b_->GetAllCanExtend(current_set);
    for (int i = 0; i < n_; i++) {
      if (current_set->find(i) == current_set->end() &&
          y1.find(i) != y1.end() && y2.find(i) != y2.end()) {
        current_set->insert(i);
        return true;
      }
    }
    auto e1 = a_->GetAllCanExchange(current_set, false);
    auto e2 = b_->GetAllCanExchange(current_set, true);
    vi q;
    q.reserve(n_);
    vi dis(n_, n_ + 1), fa(n_, -1);
    for (const auto it : y1) {
      dis[it] = 0;
      q.push_back(it);
    }
    for (int i = 0; i < (int)q.size(); i++) {
      int no = q[i];
      if (y2.find(no) != y2.end()) {
        for (int i = 0; no != -1; i = i ^ 1) {
          if (i) {
            current_set->erase(no);
          } else {
            current_set->insert(no);
          }
          no = fa[no];
        }
        return true;
      }
      const auto& re = dis[no] % 2 ? e1 : e2;
      auto it = re.find(no);
      if (it == re.end()) continue;
      for (const auto to : it->second) {
        if (dis[to] > dis[no] + 1) {
          dis[to] = dis[no] + 1;
          fa[to] = no;
          q.push_back(to);
        }
      }
    }

    return false;
  }
  bool AugmentWeighted(UI* current_set) {
    // TODO: similar as normal, but change edge weight to
    // (-w if erase the node else w) and find min distance
    return false;
  }
  int n_;
  vi weight_;
  Matroid* a_;
  Matroid* b_;
  bool (MatroidIntersection::*Augment)(UI*);
};

class AcycleGraphMatroid : public Matroid {
 public:
  AcycleGraphMatroid(int n, const vector<pii>& e) :
      Matroid(e.size()), e_(e), np_(n) {
    graph_ = vector<vi>(np_, vi{});
    for (const auto& it : e) {
      graph_[it.first].push_back(it.second);
      graph_[it.second].push_back(it.first);
    }
    for (int i = 0; i < (int)e_.size(); i++) {
      e_no_[HashE(e_[i].first, e_[i].second)] = i;
      e_no_[HashE(e_[i].second, e_[i].first)] = i;
    }
  }
  UI GetAllCanExtend(UI* current_set) override {
    vi fa = BuildDsu(current_set);
    assert(!fa.empty());
    UI ans;
    for (int i = 0; i < n_; i++) {
      if (current_set->find(i) == current_set->end()) {
        int a = e_[i].first, b = e_[i].second;
        if (GetFa(a, fa) != GetFa(b, fa)) {
          ans.insert(i);
        }
      }
    }
    return ans;
  }
  unordered_map<int, vi> GetAllCanExchange(
      UI* current_set, bool rev_edge) override {
  	vi fa(np_, -1), rfa(np_, -2), comp(np_, -1);
  	vi lca(n_, -1);
  	int current_comp = 0;
  	function<void(int)> dfs = [&](int no) {
  		comp[no] = current_comp;
  		for (const auto it : graph_[no]) if (it != fa[no]) {
  			int eno = GetEno(no, it);
  			if (current_set->find(eno) != current_set->end()) {
  				assert(fa[it] == -1);
  				lca[eno] = -2;
  				fa[it] = no;
  				rfa[it] = -1;
  				dfs(it);
  				rfa[it] = no;
  			} else {
  				if (rfa[it] != -2 && comp[it] == current_comp) {
  					lca[eno] = GetFa(it, rfa);
  				}
  			}
  		}
  	};
  	for (int i = 0; i < np_; i++) if (comp[i] == -1) {
  		rfa[i] = -1;
  		dfs(i);
  		current_comp++;
  	}

  	unordered_map<int, vi> ans;
		for (int i = 0; i < n_; i++) if (lca[i] != -2) {
			if (lca[i] == -1) {
				if (rev_edge) {
					auto& exchange = ans[i];
					for (const auto it : *current_set) {
						exchange.push_back(it);
					}
				} else {
					for (const auto it : *current_set) {
						ans[it].push_back(i);
					}
				}
			} else {
				vi rcur;
				for (int j = e_[i].first; j != lca[i]; j = fa[j]) {
					rcur.push_back(GetEno(fa[j], j));
				}
				for (int j = e_[i].second; j != lca[i]; j = fa[j]) {
					rcur.push_back(GetEno(fa[j], j));
				}
				if (rev_edge) {
					auto& exchange = ans[i];
					for (const auto it : rcur) {
						exchange.push_back(it);
					}
				} else {
					for (const auto it : rcur) {
						ans[it].push_back(i);
					}
				}
			}
		}

		return ans;
  }
  bool Oracle(UI* ui) override {
    return !BuildDsu(ui).empty();
  }

 private:
  vi BuildDsu(UI* current_set) {
    vi fa(np_, -1);

    function<bool(int, int)> uni = [&](int a, int b) {
      a = GetFa(a, fa);
      b = GetFa(b, fa);
      if (a == b) return false;
      fa[a] = b;
      return true;
    };

    for (const auto no : *current_set) {
      int a = e_[no].first, b = e_[no].second;
      if (!uni(a, b)) {
        return {};
      }
    }

    return fa;
  }
  int GetFa(int no, vi& fa) {
    if (fa[no] == -1) return no;
    return fa[no] = GetFa(fa[no], fa);
  }
  inline int GetEno(int a, int b) {
    return e_no_[HashE(a, b)];
  }
  inline ll HashE(int a, int b) {
    return 1ll * a * np_ + b;
  }

  const vector<pii>& e_;
  int np_;
  vector<vi> graph_;
  unordered_map<ll, int> e_no_;
};

