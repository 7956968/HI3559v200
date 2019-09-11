/* @(#)e_exp.c 1.6 04/04/22 */
/*
 * ====================================================
 * Copyright (C) 2004 by Sun Microsystems, Inc. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include "math.h"
#include "float.h"

static const double one    = 1.0;
static const double halF[2]    = {0.5,-0.5,};
static const double o_threshold=  7.09782712893383973096e+02;  /* 0x40862E42, 0xFEFA39EF */
static const double u_threshold= -7.45133219101941108420e+02;  /* 0xc0874910, 0xD52D3051 */
static const double ln2HI[2]   ={ 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
         -6.93147180369123816490e-01,};/* 0xbfe62e42, 0xfee00000 */
static const double ln2LO[2]   ={ 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
         -1.90821492927058770002e-10,};/* 0xbdea39ef, 0x35793c76 */
static const double invln2 =  1.44269504088896338700e+00; /* 0x3ff71547, 0x652b82fe */
static const double P1   =  1.66666666666666019037e-01; /* 0x3FC55555, 0x5555553E */
static const double P2   = -2.77777777770155933842e-03; /* 0xBF66C16C, 0x16BEBD93 */
static const double P3   =  6.61375632143793436117e-05; /* 0x3F11566A, 0xAF25DE2C */
static const double P4   = -1.65339022054652515390e-06; /* 0xBEBBBD41, 0xC5D26BF1 */
static const double P5   =  4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

static volatile double huges    = 1.0e+300;
static volatile double twom1000= 9.33263618503218878990e-302;     /* 2**-1000=0x01700000,0*/

double
__ieee754_exp(double x)    /* default IEEE double exp */
{
    double y = 0.0, hi = 0.0, lo = 0.0, c = 0.0, t = 0.0, twopk = 0.0;
    int k = 0, xsb = 0;
    unsigned int hx = 0;

    GET_HIGH_WORD(hx,x);
    xsb = (hx>>31)&1;        /* sign bit of x */
    hx &= 0x7fffffff;        /* high word of |x| */

    /* filter out non-finite argument */
    if(hx >= 0x40862E42) {            /* if |x|>=709.78... */
        if(hx>=0x7ff00000) {
            unsigned int lx;
            GET_LOW_WORD(lx,x);
            if(((hx&0xfffff)|lx)!=0)
                return x+x;         /* NaN */
            else return (xsb==0)? x:0.0;    /* exp(+-inf)={inf,0} */
        }
        if(x > o_threshold) return huges*huges; /* overflow */ /*lint !e564*/
        if(x < u_threshold) return twom1000*twom1000; /* underflow */ /*lint !e564*/
    }

    /* argument reduction */
    if(hx > 0x3fd62e42) {        /* if  |x| > 0.5 ln2 */
        if(hx < 0x3FF0A2B2) {    /* and |x| < 1.5 ln2 */
        hi = x-ln2HI[xsb]; lo=ln2LO[xsb]; k = 1-xsb-xsb;
        } else {
        k  = (int)(invln2*x+halF[xsb]);
        t  = k;
        hi = x - t*ln2HI[0];    /* t*ln2HI is exact here */
        lo = t*ln2LO[0];
        }
        STRICT_ASSIGN(double, x, hi - lo);
    }
    else if(hx < 0x3e300000)  {    /* when |x|<2**-28 */
        if((huges+x)>one) return one+x;/* trigger inexact */
    }
    else k = 0;

    /* x is now in primary range */
    t  = x*x;
    if(k >= -1021)
        INSERT_WORDS(twopk,0x3ff00000+(k<<20), 0);
    else
        INSERT_WORDS(twopk,0x3ff00000+((k+1000)<<20), 0);
    c  = x - t*(P1+t*(P2+t*(P3+t*(P4+t*P5))));
    if(k==0)     return one-((x*c)/(c-2.0)-x);
    else         y = one-((lo-(x*c)/(2.0-c))-hi);
    if(k >= -1021) {
        if (k==1024) return y*2.0*0x1p1023;
        return y*twopk;
    } else {
        return y*twopk*twom1000;
    }
}

double exp(double x)
{
    return __ieee754_exp(x);
}

double expm1(double x) {
    return exp(x) - 1.0;
}
