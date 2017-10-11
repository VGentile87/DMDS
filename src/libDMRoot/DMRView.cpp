//-- Authors :  AA,VT   19/06/2016
#include "TMath.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "DMRLog.h"
#include "DMRView.h"
#include "DMRCluster.h"
#include "DMRGrain.h"
#include "DMRMicrotrack.h"
using namespace TMath;

ClassImp(DMRView)
//______________________________________________________________________________
DMRView::DMRView()
{
  HD   = new DMRViewHeader();
  CL   = new TClonesArray("DMRCluster",    5000);
  GR   = new TClonesArray("DMRGrain",      5000);
  MT   = new TClonesArray("DMRMicrotrack",  200);
  IM   = new TClonesArray("DMRImageCl",    5000);
  FR   = new TClonesArray("DMRFrame",       128);
}

//______________________________________________________________________________
DMRView::~DMRView()
{
  //  Clear();

  if(HD)  {               delete HD; HD=0; }
  if(CL)  { CL->Delete(); delete CL; CL=0; }
  if(GR)  { GR->Delete(); delete GR; GR=0; }
  if(MT)  { MT->Delete(); delete MT; MT=0; }
  if(IM)  { IM->Delete(); delete IM; IM=0; }
  if(FR)  { FR->Delete(); delete FR; FR=0; }
}

//______________________________________________________________________________
void DMRView::Clear()
{
  if(HD)  HD->Set0();
  if(CL)  CL->Delete();
  if(GR)  GR->Delete();
  if(MT)  MT->Delete();
  if(IM)  IM->Delete();
  if(FR)  FR->Delete();
  redHD=redCL=redGR=redMT=redIM=redFR=false;
}

//______________________________________________________________________________
DMRCluster *DMRView::AddCluster()
{
  DMRCluster *c = (DMRCluster*)(new((*CL)[CL->GetLast()+1]) DMRCluster() );
  c->id = CL->GetLast();
  return c;
}
DMRGrain *DMRView::AddGrain()
{
  DMRGrain *g = (DMRGrain*)(new((*GR)[GR->GetLast()+1]) DMRGrain() );
  g->id=GR->GetLast();
  return g;
}
DMRMicrotrack *DMRView::AddMicrotrack()
{
  DMRMicrotrack *mt= (DMRMicrotrack*)(new((*MT)[MT->GetLast()+1]) DMRMicrotrack() );
  mt->id=MT->GetLast();
  return mt;
}
DMRImageCl *DMRView::AddImageCl()
{
  DMRImageCl *im = (DMRImageCl*)   (new((*IM)[IM->GetLast()+1]) DMRImageCl() );
  im->id=IM->GetLast();
  return im;
}
DMRFrame *DMRView::AddFrame()
{
  DMRFrame *f = (DMRFrame*)(new((*FR)[FR->GetLast()+1]) DMRFrame() );
  f->id = FR->GetLast();
  return f;
}

//______________________________________________________________________________
DMRFrame* DMRView::GetFR( Int_t iz, int ipol ) const
{
  int nfr  = FR->GetEntries();
  for(int i=0; i<nfr; i++ )
  {
    DMRFrame *f = GetFR(i);
    if( f->iz==iz && f->ipol==ipol ) return f;
  }
  return 0;
}

//______________________________________________________________________________
TObjArray* DMRView::GetCLofGR( int igr )
{
  TObjArray *arr = new TObjArray();
  int ncl  = CL->GetEntries();
  for(int i=0; i<ncl; i++ )
  {
    DMRCluster *c = GetCL(i);
    if( c->igr==igr ) arr->Add(c);
  }
  return arr;
}

//______________________________________________________________________________
TObjArray* DMRView::GetClusters( float x0, float y0, float dx, float dy, Int_t iz, int ipol )
{
  printf("getcl: %.2f %.2f %.2f %.2f %d %d\n",x0,y0,dx,dy,iz,ipol);
  TObjArray *arr = new TObjArray();
  int ncl  = CL->GetEntries();
  for(int i=0; i<ncl; i++ )
  {
    DMRCluster *c = GetCL(i);
    DMRFrame   *fr = GetFR(c->ifr);
    if( fr->iz==iz && c->ipol==ipol && Abs(c->x-x0)<dx && Abs(c->y-y0)<dy ) arr->Add(c);
  }
  return arr;
}

//______________________________________________________________________________
TObjArray* DMRView::GetFrames( Int_t ifr )
{
  TObjArray *arr = new TObjArray();
  int nfr  = FR->GetEntries();
  TArrayF ap(nfr);
  TArrayI ind(nfr);
  int cnt=0;
  for(int i=0; i<nfr; i++ )
  {
    DMRFrame *f = GetFR(i);
    if( f->ID()==ifr ) {
      arr->Add(f);
      ap[cnt] = f->pol;
      ind[cnt] = cnt;
      cnt++;
    }
  }
  Sort( cnt, ap.GetArray(), ind.GetArray(), 0 );
  TObjArray *sarr = new TObjArray();
  for(int i=0; i<cnt; i++ ) sarr->Add( arr->At(ind[i]) );
  delete arr;
  return sarr;
}

//______________________________________________________________________________
DMRImageCl *DMRView::GetImageCl(const DMRCluster &cl, int dx, int dy)
{
  DMRImageCl      *result=0;
  DMRImageCl      *imc = GetIM(cl.img);
  if( imc && imc->GetImage() && imc->cols>=2*dx && imc->rows>=2*dy)
    result = imc->ExtractFragment( (imc->cols+1)/2, (imc->rows+1)/2, dx, dy);
  else 
  {
    DMRFrame *fr=GetFR(cl.ifr);
    if(fr)
    {
      result = fr->ExtractFragment( imc->x+(imc->cols+1)/2, imc->y+(imc->rows+1)/2, dx, dy);
    }
  }
  return result;
}

//______________________________________________________________________________
void DMRView::AddIM2FR( int ncol, int nrow )
{
  int nfr  = FR->GetEntries();
  int nimg = IM->GetEntries();
  for( int ifr=0; ifr<nfr; ifr++)
  {
    DMRFrame *fr = GetFR(ifr);
    if(fr)
    {
      if(!(fr->GetImage()))    // do it only if own frame image was not defined
      {
        fr->SetImage(ncol,nrow);
        int ncl = CL->GetEntries();
        for(int i=0; i<ncl; i++)
        {
          DMRCluster      *cl = GetCL(i);
          if( cl && cl->ifr==fr->ID() && cl->img>=0 && i<nimg && cl->ipol==fr->ipol )
          {
            DMRImageCl      *im = GetIM(cl->img);
            if( im )    fr->AddIM2FR( *im );
          }
        }
        fr->CalculateImageStatistics();
      }
    }
  }
}


//______________________________________________________________________________
void DMRView::FixEncoderFaults()
{
  // here is assumed that in the same view frames must have exactly the same coordiante
  int ncl  = CL->GetEntries();
  for( int icl=0; icl<ncl; icl++)
  {
    DMRCluster *cl = GetCL(icl);
    if(cl)
    {
      DMRFrame *fr   = GetFR(cl->ifr);
      if(fr)
      {
        cl->x -= fr->x-HD->x;
        cl->y -= fr->y-HD->y;
      }
    }
  }
  
  float dxm[2]={9999999,-9999999};
  float dym[2]={9999999,-9999999};
  int nfr  = FR->GetEntries();
  for( int ifr=0; ifr<nfr; ifr++)
  {
    DMRFrame *fr = GetFR(ifr);
    if(fr)
    {
      float dx = fr->x-HD->x;
      float dy = fr->y-HD->y;
      dxm[0] = Min(dx,dxm[0]);
      dym[0] = Min(dy,dym[0]);
      dxm[1] = Max(dx,dxm[1]);
      dym[1] = Max(dy,dym[1]);
      fr->x -= dx;
      fr->y -= dy;
    }
  }
  Log(1,"DMRView::FixEncoderFaults"," dx[%.1f %.1f] dy[%.1f %.1f] [nm]",
        dxm[0]*1000, dxm[1]*1000, dym[0]*1000, dym[1]*1000 );
}

