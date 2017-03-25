#ifndef POLY_H
#define POLY_H
#include <string.h>
class Poly
{
    int n;
    int *c;
public:
    Poly(){}
    Poly(int power){    //max power of polynomial
        n=power;
        c=new int(n+1);
        memset(c,0,n*sizeof(int));
    }
    Poly(int a1,int a2,int a3,int a4){
        c=new int(4);
        c[0]=a1;
        c[1]=a2;
        c[2]=a3;
        c[3]=a4;
    }

    Poly(int power,int ratio[]) {
        n=power;
        c=new int(n+1);
        for(int i=0;i<n+1;++i){
            c[i]=ratio[i];
        }
        this->simplify();
    }

    int& operator[](int i){
        return c[i];
    }
    double value(double v){
        double x=1,rst=0;
        for(int i=0;i<n+1;++i){
            rst+=x*c[i];
            x*=v;
        }
        return rst;
    }

    const Poly& diff() const{
        Poly *p=new Poly(n-1);
        for(int i=1;i<n+1;++i){
            (*p)[i-1]=c[i]*i;
        }
        p->simplify();
        return *p;
    }
    const Poly& operator*(int m) const{
        Poly *p=new Poly(n);
        for(int i=0;i<n+1;++i){
            (*p)[i]=c[i]*m;
        }
        p->simplify();
        return *p;
    }

    const Poly& operator+(Poly p) const{
        int m=qMax(n,p.power());
        Poly *sum=new Poly(m);
        for(int i=0;i<m+1;++i){
            (*sum)[i]=c[i]+p[i];
        }
        sum->simplify();
        return *sum;
    }
    int power(){
        return n;
    }
    void simplify(){
        for(int i=n;i>0;--i){
            if(c[i]!=0) break;
            n--;
        }
    }
};
#endif // POLY_H
