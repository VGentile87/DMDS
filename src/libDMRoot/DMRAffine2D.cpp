//-- Authors :  VT   4/11/2016
#include <limits>
#include <TROOT.h>
#include <TMath.h>
#include "DMRLog.h"
#include "DMRAffine2D.h"

ClassImp(DMRAffine2D)

using namespace TMath;
inline Double_t sqr(Double_t x) {return (x*x);};

//______________________________________________________________________________
DMRAffine2D::DMRAffine2D()
{
  a11=1; a22=1;
  a12=0; a21=0;
  b1=0;  b2=0;
}

//______________________________________________________________________________
void DMRAffine2D::Transform( DMRAffine2D a )
{
  Set(
     a.a11*a11 + a.a12*a21,
     a.a11*a12 + a.a12*a22,
     a.a21*a11 + a.a22*a21,
     a.a21*a12 + a.a22*a22,
     a.a11*b1  + a.a12*b2 + a.b1,
     a.a21*b1  + a.a22*b2 + a.b2
     );
}

//______________________________________________________________________________
void DMRAffine2D::Set(const char *str)
{
  Float_t ta11=1, ta12=0, ta21=0, ta22=1, tb1=0,tb2=0;
  if(str)
    if(sscanf(str, "%f %f %f %f %f %f", &ta11, &ta12, &ta21, &ta22, &tb1, &tb2 ) == 6 )
      Set( ta11, ta12, ta21, ta22, tb1, tb2);
}

//______________________________________________________________________________
const char *DMRAffine2D::AsString() const
{
  return Form("%9.6f %9.6f %9.6f %9.6f %12.6f %12.6f", a11, a12, a21, a22, b1, b2 );
}

//______________________________________________________________________________
void DMRAffine2D::Rotate(Float_t angle)
{
  DMRAffine2D t;
  t.Set( Cos(angle), -Sin(angle), Sin(angle), Cos(angle), 0, 0 );
  Transform( t );
}

//______________________________________________________________________________
void DMRAffine2D::Invert()
{
  Double_t d = a11*a22 - a12*a21;
  if(Abs(d)>DBL_MIN)
    Set (
         a22/d,
        -a12/d,
        -a21/d,
         a11/d,
         (a12*b2 - a22*b1)/d,
         (a21*b1 - a11*b2)/d
        );
}

//______________________________________________________________________________
Double_t DMRAffine2D::Phi(Double_t x, Double_t y) const 
{ 
  // phi() is defined in [0,TWOPI]
  return TMath::Pi()+TMath::ATan2(-y,-x);
}

//______________________________________________________________________________
Int_t DMRAffine2D::Calculate(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1, int flag)
{
  // Calculate affine transformation for 2 patterns
  // if flag==2 calculate only rotation and translation without scaling

  if(flag==2) return CalculateTurn( n, x0,y0, x1,y1 );
  else        return CalculateFull( n, x0,y0, x1,y1 );
}

//______________________________________________________________________________
Int_t DMRAffine2D::CalculateFull(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1)
{
  //
  // Calculate affine transformation for 2 patterns
  // b1 == b0->Transform(this);
  //

  Double_t a,b,c,d,p,q;
  a=b=c=d=p=q=0;

  if(n<2)    return 0; 

  else if(n==2){

    Double_t    dx0 = x0[1] - x0[0];
    Double_t    dy0 = y0[1] - y0[0];
    Double_t    dx1 = x1[1] - x1[0];
    Double_t    dy1 = y1[1] - y1[0];

    Double_t    s = TMath::Sqrt(dx1*dx1+dy1*dy1)/TMath::Sqrt(dx0*dx0+dy0*dy0);
    Double_t    th = Phi(dx1,dy1)-Phi(dx0,dy0);

    a =  s*TMath::Cos(th);
    b = -s*TMath::Sin(th);
    c =  s*TMath::Sin(th);
    d =  s*TMath::Cos(th);
    p =  x1[0] - a*x0[0] - b*y0[0];
    q =  y1[0] - c*x0[0] - d*y0[0];

  }
  else {

    Double_t  
        sx0   =0,
    sy0   =0,
    sx1   =0,  
    sy1   =0,
    sx0x0 =0,  
    sy0y0 =0,  
    sx0y0 =0,  
    sx0x1 =0,  
    sx0y1 =0,  
    sx1y0 =0,  
    sy0y1 =0,
    r     =0;

    for(int i=0; i<n; i++) {
      r = 1./(i+1);
      sx0   = sx0   *i*r + x0[i]         *r;
      sy0   = sy0   *i*r + y0[i]         *r;
      sx1   = sx1   *i*r + x1[i]         *r;
      sy1   = sy1   *i*r + y1[i]         *r;
      sx0x0 = sx0x0 *i*r + x0[i] * x0[i] *r;
      sy0y0 = sy0y0 *i*r + y0[i] * y0[i] *r;
      sx0y0 = sx0y0 *i*r + x0[i] * y0[i] *r;
      sx0x1 = sx0x1 *i*r + x0[i] * x1[i] *r;
      sx0y1 = sx0y1 *i*r + x0[i] * y1[i] *r;
      sx1y0 = sx1y0 *i*r + x1[i] * y0[i] *r;
      sy0y1 = sy0y1 *i*r + y0[i] * y1[i] *r;
    }
    
    Double_t    sp0 = sx0x1 - sx0*sx1;
    Double_t    sp1 = sx1y0 - sx1*sy0;
    Double_t    sq0 = sx0y1 - sx0*sy1;
    Double_t    sq1 = sy0y1 - sy0*sy1;

    Double_t    sar = sx0x0 - sx0*sx0;
    Double_t    sbr = sx0y0 - sx0*sy0;
    Double_t    scr = sbr;
    Double_t    sdr = sy0y0 - sy0*sy0;
    Double_t    det = sar*sdr-sbr*scr;

    if ( det <= std::numeric_limits<double>::epsilon() )
    {
      Log(1,"DMRAffine2D::CalculateFull","WARNING! determinant is too small");
      return 0;
    }

    Double_t    sa =  sdr/det;
    Double_t    sb = -sbr/det;
    Double_t    sc = -scr/det;
    Double_t    sd =  sar/det;

    a = sa*sp0+sb*sp1;
    b = sc*sp0+sd*sp1;
    c = sa*sq0+sb*sq1;
    d = sc*sq0+sd*sq1;
    p = sx1-a*sx0-b*sy0;
    q = sy1-c*sx0-d*sy0;
  }

  Log(3,"CalculateFull","Aff2D( %6d ): %9.6f %9.6f %9.6f %9.6f %12.6f %12.6f", 
      n, a,b,c,d,p,q);

  Set(a,b,c,d,p,q);

  return  1;
}

//______________________________________________________________________________
Int_t DMRAffine2D::CalculateTurn(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1)
{
  //
  // Calculate affine transformation for 2 patterns: shift and rotation only
  // b1 == b0->Transform(this);
  //

  Double_t a,b,c,d,p,q;
  a=b=c=d=p=q=0;
  Double_t  X = 0.0;
  Double_t  Y = 0.0;
  Double_t  X_ = 0.0;
  Double_t  Y_ = 0.0;
  Double_t  f = 0.0;
  Double_t  e = 0.0;
  Double_t  teta1,teta2,a1,a2,b1,b2,F1=0,F2=0;
  Int_t i;
    

  if (n==0){
    return 0;
  }
  else if(n==1){
    a = 1.0;
    b = 0.0;
    c = 0.0;
    d = 1.0;
    p = x1[0] - x0[0];
    q = y1[0] - y0[0];
  }
  else {
    for (i=0;i<n;i++) {
      X += x0[i];
      Y += y0[i];
      X_ += x1[i];
      Y_ += y1[i];
    }
    X /= (Double_t)n;
    Y /= (Double_t)n;
    X_ /= (Double_t)n;
    Y_ /= (Double_t)n;

    for (i=0;i<n;i++) {
      f += x1[i] * (x0[i] - X) + y1[i] * (y0[i] - Y);
      d += x1[i] * (Y - y0[i]) + y1[i] * (x0[i] - X);
      e += x1[i] * Y_ - y1[i] * X_;
    }
    
    teta1 = TMath::ASin((e*f-d*TMath::Sqrt(sqr(f)+sqr(d)-sqr(e)))/(sqr(f)+sqr(d)));
    teta2 = TMath::Pi() - TMath::ASin((e*f+d*TMath::Sqrt(sqr(f)+sqr(d)-sqr(e)))/(sqr(f)+sqr(d)));

    a1 = X + Y_*TMath::Sin(teta1) - X_*TMath::Cos(teta1);
    b1 = Y - Y_*TMath::Cos(teta1) - X_*TMath::Sin(teta1);

    a2 = X + Y_*TMath::Sin(teta2) - X_*TMath::Cos(teta2);
    b2 = Y - Y_*TMath::Cos(teta2) - X_*TMath::Sin(teta2);

    for (i=0;i<n;i++) {
      F1 += sqr(x0[i] - x1[i]*TMath::Cos(teta1) + y1[i]*TMath::Sin(teta1) - a1) \
          + sqr(y0[i] - x1[i]*TMath::Sin(teta1) - y1[i]*TMath::Cos(teta1) - b1);

      F2 += sqr(x0[i] - x1[i]*TMath::Cos(teta2) + y1[i]*TMath::Sin(teta2) - a2) \
          + sqr(y0[i] - x1[i]*TMath::Sin(teta2) - y1[i]*TMath::Cos(teta2) - b2);
    }

    if (F1<F2) {
      a = TMath::Cos(teta1);
      b = TMath::Sin(teta1);
      c = -TMath::Sin(teta1);
      d = TMath::Cos(teta1);
      p = -a1*TMath::Cos(teta1) - b1*TMath::Sin(teta1);
      q = a1*TMath::Sin(teta1) - b1*TMath::Cos(teta1);
    }
    else {
      a = TMath::Cos(teta2);
      b = TMath::Sin(teta2);
      c = -TMath::Sin(teta2);
      d = TMath::Cos(teta2);
      p = -a2*TMath::Cos(teta2) - b2*TMath::Sin(teta2);
      q = a2*TMath::Sin(teta2) - b2*TMath::Cos(teta2);
    }
  }

  Log(3,"CalculateTurn","Aff2D( %6d ): %9.6f %9.6f %9.6f %9.6f %12.6f %12.6f", 
      n, a,b,c,d,p,q);

  Set(a,b,c,d,p,q);
  return  1;
}
