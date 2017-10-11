//-- Author :  VT   15/02/2017
#include "DMRFiducial.h"

ClassImp(DMRFiducialMark)
ClassImp(DMRMarksSet)

//______________________________________________________________________________
DMRMarksSet::DMRMarksSet()
{
  eMarks.SetOwner();
}

//______________________________________________________________________________
DMRFiducialMark* DMRMarksSet::NewMark()
{
  DMRFiducialMark *m = new DMRFiducialMark();
  m->cnt = eMarks.GetEntries();
  eMarks.Add(m);
  return m;
}

//______________________________________________________________________________
DMRAffine2D  *DMRMarksSet::Abs2Stage( bool invert ) const
{ 
  DMRAffine2D *aff = new DMRAffine2D();
  int n = N();
  Double_t *xa = new Double_t[n];
  Double_t *ya = new Double_t[n];
  Double_t *xm = new Double_t[n];
  Double_t *ym = new Double_t[n];
  int nfnd = 0;
  for(int i=0; i<n; i++)
  {
    DMRFiducialMark *m = GetMark(i);
    if(m->is_found) 
    {
      xa[nfnd] = m->x_abs;
      ya[nfnd] = m->y_abs;
      xm[nfnd] = m->x_msr;
      ym[nfnd] = m->y_msr;
      nfnd++;
    }
  }
  if(invert) aff->Calculate( nfnd, xm,ym, xa,ya );
  else       aff->Calculate( nfnd, xa,ya, xm,ym );
  return aff;
}
