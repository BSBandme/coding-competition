struct Point {
	union {
		double co[2];
		struct {
			double x, y;
		};
	};
	double ang;
	Point(double a = 0, double b = 0) : x(a), y(b) {ang = 0;}

	Point operator + (const Point& a) const {
		Point ans;
		ans.x = x + a.x;
		ans.y = y + a.y;
		return ans;
	}
	Point operator - (const Point& a) const {
		Point ans;
		ans.x = x - a.x;
		ans.y = y - a.y;
		return ans;
	}
	double operator * (const Point& a) const {
		return x * a.x + y * a.y;
	}
	double operator % (const Point& a) const {
		return x * a.y - y * a.x;
	}
	Point operator * (const double p) const {
		Point ans;
		ans.x = x * p;
		ans.y = y * p;
		return ans;
	}

};
double GetAngle(const double x, const double y) {
	if(x == 0) {
		if(y > 0) return pi / 2;
		else return pi / 2 * 3;
	}
	if(x > 0) {
		if(y < 0) return atan(y / x) + pi * 2;
		else return atan(y / x);
	} else {
		return atan(y / x) + pi;
	}
}
double GetAngle(const Point& p) {
	return GetAngle(p.x, p.y);
}
bool In(const Point& cen, const Point& a, const Point& b) {
	return jud((a - cen) % (b - cen), 0) >= 0;
}

//判断两线段是否相交
bool Inter(
		const Point& a, const Point& b, const Point& c, const Point& d){
	if ( min(a.x, b.x) > max(c.x, d.x) ||
	min(a.y, b.y) > max(c.y, d.y) ||
	min(c.x, d.x) > max(a.x, b.x) ||
	min(c.y, d.y) > max(a.y, b.y) ) return 0;
	double h, i, j, k;
	h = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	i = (b.x - a.x) * (d.y - a.y) - (b.y - a.y) * (d.x - a.x);
	j = (d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x);
	k = (d.x - c.x) * (b.y - c.y) - (d.y - c.y) * (b.x - c.x);
	return h * i <= eps && j * k <= eps;
}
struct Line {
	Point a, b;
	double ang;
	Line(const Point& a1 = Point(0, 0), const Point& b1 = Point(1, 0)): a(a1), b(b1) {
		Point temp = b1 - a1;
		ang = GetAngle(temp.x, temp.y);
	}
	friend bool Contain(Line l, Point a);
	bool operator == (const Line &rline) const {
		return Contain(*this, rline.a) && Contain(*this, rline.b);
	}
};
inline bool Contain(const Line& l, const Point& a) {
	return jud((a - l.a) % (l.b - l.a), 0) == 0;
}
Point GetInter(const Line& la, const Line& lb) {
	double sa = (lb.a - la.a) % (lb.a - la.b);
	double sb = (lb.b - la.b) % (lb.b - la.a);
	Point ans = (lb.a * sb + lb.b * sa) *(1.0 / (sb + sa));
	return ans;
}