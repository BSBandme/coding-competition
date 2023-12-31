namespace Poly
{
    const long double pi=acosl(-1);
    struct C
    {
        long double x,y;
        C():x(0),y(0){}
        C(long double a,long double b):x(a),y(b){}
        C operator+(const C &b){return C(x+b.x,y+b.y);}
        C operator-(const C &b){return C(x-b.x,y-b.y);}
        C operator*(const C &b){return C(x*b.x-y*b.y,x*b.y+y*b.x);}
        C operator/(long double b){return C(x/b,y/b);}
    };
    int id[N];
    int init(int n)
    {
        int ret=1;
        for (;1<<ret<n;ret++);
        for (int i=0;i<1<<ret;i++)
            id[i]=id[i>>1]>>1|((i&1)<<(ret-1));
        return 1<<ret;
    }
    C getwl(int x,int tag)
    {
        return C(cosl(2*pi/x),tag*sinl(2*pi/x));
    }
    void fft(C *a,int len,int tag=1)
    {
        for (int i=0;i<len;i++)
            if (id[i]>i)
                std::swap(a[i],a[id[i]]);
        for (int l=1;l<len;l<<=1)
        {
            C wl=getwl(l*2,tag);
            for (int st=0;st<len;st+=l<<1)
            {
                C w(1,0),tmp;
                for (int i=st;i<st+l;i++,w=w*wl)
                    tmp=a[i+l]*w,a[i+l]=a[i]-tmp,a[i]=a[i]+tmp;
            }
        }
        if (tag<0)
            for (int i=0;i<len;i++)
                a[i]=a[i]/len;
    }
    int mul(int *ret,int *a,int alen,int *b,int blen,int M)
    {
        static C c[N],d[N],e[N];
        int s=alen+blen-1,len=init(s);
        for (int i=0;i<alen;i++)
            c[i]=C(a[i]>>15,a[i]&0x7fff);
		std::fill(c+alen,c+len,C());
        for (int i=0;i<blen;i++)
            d[i]=C(b[i]>>15,b[i]&0x7fff);
		std::fill(d+blen,d+len,C());
        fft(c,len),fft(d,len);
        for (int i=0;i<len;i++)
        {
            int j=(len-i)&(len-1);
            e[i]=d[i]*C(0.5*(c[i].x+c[j].x),0.5*(c[i].y-c[j].y));
            d[i]=d[i]*C(0.5*(c[i].y+c[j].y),0.5*(c[j].x-c[i].x));
        }
        fft(e,len,-1),fft(d,len,-1);
        for (int i=0;i<s;i++)
        {
            ll x=roundl(e[i].x),y=roundl(e[i].y),z=roundl(d[i].x),w=roundl(d[i].y);
            ret[i]=(((x%M<<30)%M+((y+z)%M<<15)%M+w%M)%M+M)%M;
        }
        return s;
    }
}