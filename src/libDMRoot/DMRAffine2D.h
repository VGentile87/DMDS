#ifndef _DMRAFFINE2D_H_
#define _DMRAFFINE2D_H_

#include <TObject.h>

class DMRAffine2D : public TObject
{
  public:
    Double_t a11,a12,a21,a22,b1,b2;
  
  public:
    DMRAffine2D();
    virtual  ~DMRAffine2D(){};
    
    void Set( const char *str);
    void Set( double _a11, double _a12, double _a21, double _a22, double _b1, double _b2)
    {a11=_a11; a12=_a12; a21=_a21; a22=_a22; b1=_b1; b2=_b2; }
    Double_t X(Double_t x, Double_t y) { return x*a11+y*a12+b1; }
    Double_t Y(Double_t x, Double_t y) { return x*a21+y*a22+b2; }
    void     Transform(DMRAffine2D a);
    void     Invert();
    void     Rotate(float angle);
    const char *AsString() const;
    Int_t Calculate(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1, int flag=0);
    Int_t CalculateFull(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1);
    Int_t CalculateTurn(int n, Double_t *x0, Double_t *y0, Double_t *x1, Double_t *y1);
    Double_t Phi(Double_t x, Double_t y) const;

    ClassDef(DMRAffine2D,1);
};

#endif
