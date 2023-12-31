
// 计算  x/ta + y/tb + z/tc = 1 的非负整数解个数（范围1e18）
void getgong2(long long ta, long long tb, long long tc) {
	ll gab = __gcd(ta, tb);
	pll tab = mpr(ta, tb);
	tab.first /= gab;
	ll gctab = getgcd(tc, tab);
	ll ax, by;
	extend_gcd(tb, ta, ax, by);
//	bint btab(tab.first);
//	btab = btab * bint(tab.second);
	pll leftab = tab;
	ll rgg = __gcd(leftab.first, gctab);
	leftab.first /= rgg;
	leftab.second /= __gcd(leftab.second, gctab / rgg);
//	bint bleftab(leftab.first);
//	bleftab = bleftab * bint(leftab.second);
	ll fenmux = ta / gab, fenmuy = tb / gab;
	rgg = getgcd(fenmux, leftab);
	ll dancix = fenmux / rgg;
	rgg = getgcd(fenmuy, leftab);
	ll danciy = fenmuy / rgg;
	ll tans = (gctab + 1) % mod * (tb % mod * (abs(ax) % mod) % mod + 1) % mod;
	if(ax > 0) {
		tans += (gctab / dancix + 1) % mod;
		tans %= mod;
	} else  {
		tans -= (gctab / dancix + 1) % mod;
		if(tans < 0) tans += mod;
	}
	if(tans % 2) tans += mod;
	tans /= 2;
	if(ax <= 0) tans = mod - tans;
	ll rtans = (gctab + 1) % mod * (ta % mod * (abs(by) % mod) % mod + 1) % mod;
	if(by > 0) {
		rtans += (gctab / danciy + 1) % mod;
		rtans %= mod;
	} else  {
		rtans -= (gctab / danciy + 1) % mod;
		if(rtans < 0) rtans += mod;
	}
	if(rtans % 2) rtans += mod;
	rtans /= 2;
	if(by <= 0) rtans = mod - rtans;
	gong = rtans + tans;
	gong %= mod;

//	cerr << 1 << endl;
}

