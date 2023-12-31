//已知x, y求a,b 使 ax + by = gcd(x, y)
void extend_gcd(long long num1, long long num2, long long &a, long long &b) {
	long long i, j;
 	if(!(num1 % num2)) {
		a = 1;
		b = -num1 / num2 + 1;
		return;
	}
	extend_gcd(num2, num1 % num2, i, j);
	a = j;
	b = i - j * (num1 / num2);
}

//计算从0开始加add, 并模mod, be-en中这些数第一个出现的数是什么
ll getfirst(ll be, ll en, ll mod, ll add) {
	ll rans;
	if(add == 0) return -1;
	if(add > mod / 2) {
		rans = getfirst(mod - en, mod - be, mod, mod - add);
		if(rans == -1) return -1;
		rans = mod - rans;
		return rans;
	}
	if((be - 1) / add != en / add) {
		ll k = (be - 1) / add + 1;
		rans = k * add;
		assert(rans >= be && rans <= en);
		return rans;
	}
	ll radd = add - mod % add;
	ll rbe = be % add, ren = en % add;
	rans = getfirst(rbe, ren, add, radd);
	if(rans == -1) return -1;
	rans += be - rbe;
	return rans;
}

//计算从be开始加add, 并模mod, 最多加left次，最小可得到的数
ll getmin(ll be, ll mod, ll add, ll left) {
	if(left == 0) return be;
	pll radd = mpr(getfirst(mod - be, mod - 1, mod, add), 0);
	if(radd.first == -1) return be;
	ll ra, rb, gg = __gcd(mod, add), xia = add / gg, xib = mod / gg;
	extend_gcd(mod, add, ra, rb);
	if(rb < 0) {
		rb += xib;
		ra -= xia;
	}
	ra = ra * (radd.first / gg % xia) % xia;
	rb = rb * (radd.first / gg % xib) % xib;
	assert(rb * add + mod * ra == radd.first);
	radd.second = rb;
	if(radd.second > left) return be;
	radd.first = be - (mod - radd.first);
	left -= radd.second;

	ll rpadd = (radd.first / (be - radd.first));
	if(1.0 * rpadd * radd.second > left)
		return radd.first - (left / radd.second) * (be - radd.first);
	radd.first -= rpadd * (be - radd.first);
	left -= rpadd * radd.second;
	return getmin(radd.first, mod, add, left);
}

template <class c>
inline c div1(c bei, c chu){
	if(chu == 0) return -1;
	if(bei % chu == 0) return bei / chu;
	if(gcd1(chu, mod) != 1) return -1;
	long long a, b;
	extend_gcd(mod, chu, a, b);
	b *= bei;
	if(b < 0) b = -((-b) % mod) + mod;
	else b = b % mod;
	return b;
}
