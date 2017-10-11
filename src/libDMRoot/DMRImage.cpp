//-- Authors :  AA,VT   19/06/2016
#include <TH2F.h>
#include <TMath.h>
//#include <TASImage.h>
#include "DMRImage.h"
#include "DMRLog.h"
ClassImp(DMRImage)
ClassImp(DMRImageCl)
ClassImp(DMRFrame)
ClassImp(DMRFrameRaw)

using namespace TMath;
//______________________________________________________________________________
DMRImage::DMRImage():TObject(),eImg(NULL)
{
  cols=rows=0;
  sensor=0;
  color=0;
  pol=0;
  ipol=0;
  npx=0;
  pxvol=0;
  thres=ll=0;
};

//______________________________________________________________________________
DMRImage::~DMRImage()
{
  if(eImg)
    delete eImg;
}

//______________________________________________________________________________
void DMRImage::SetImage( Int_t cols_, Int_t rows_, const Byte_t* img)
{
  cols=cols_;
  rows=rows_;
  int bytes = cols*rows;
  char *buffer = new char[bytes];
  if(eImg) delete eImg;
  eImg = new TArrayC();
  memcpy((void *)buffer,(const void *)img,bytes);
  eImg->Adopt(bytes,buffer);
}

//______________________________________________________________________________
void DMRImage::SetImage( Int_t cols_, Int_t rows_)
{
  cols=cols_;
  rows=rows_;
  if(eImg) delete eImg;
  int bytes = cols*rows;
  eImg = new TArrayC(bytes);
}

//______________________________________________________________________________
void DMRImage::RemovePixelNoise( Int_t delta )
{
  int xr[8]={1,0,-1, 0, 1, 1,-1,-1};
  int yr[8]={0,1, 0,-1, 1,-1, 1,-1};
  for(int ix=0; ix<cols; ix++)
    for(int iy=0; iy<rows; iy++)
  {
    bool is_spike=true;
    bool is_hole=true;
    int mean=0;
    int cnt=0;
    int p0 = GetPixel(ix,iy);
    for(int j=0; j<8; j++)
    {
      int ind=I(ix+xr[j] , iy+yr[j]);
      if(ind>=0)
      {
        int p = GetPixel( ind );
        mean += p;
        cnt++;
        if( p0-p < delta )          is_spike=false;
        if( p-p0 < delta )          is_hole =false;
        if( !is_spike && !is_hole )  break;
      }
    }
    if( (is_spike||is_hole)&&cnt)
    {
      if( is_spike) printf("remove spike(%d,%d): %d -> %d\n",ix,iy,p0,mean/cnt);
      if( is_hole ) printf("remove  hole(%d,%d): %d -> %d\n",ix,iy,p0,mean/cnt);
      SetPixel(ix,iy, mean/cnt );
    }
  } 
}

//______________________________________________________________________________
void DMRImage::ExtractFragment(DMRImage &im, int x0, int y0, int dx, int dy )
{
  //Log(2,"DMRImage::ExtractFragment","%d +-%d, %d +-%d",x0,dx,y0,dy);
  im.SetImage(2*dx,2*dy);
  for(int ix=x0-dx; ix<=x0+dx; ix++)
    for(int iy=y0-dy; iy<=y0+dy; iy++)
  {
    int ind=I(ix , iy);
    if(ind>=0)
    {
      im.SetPixel( ix-x0+dx, iy-y0+dy, GetPixel( ind ));
    }
  }
  im.sensor = sensor;
  im.color  = color;
  im.pol    = pol;
  im.ipol   = ipol;
  im.ll     = ll;
  im.thres  = thres;
}

//______________________________________________________________________________
void DMRImage::Min( const DMRImage &im )
{
  //assumed equal images
  for(int ix=0; ix<cols; ix++)
    for(int iy=0; iy<rows; iy++)
  {
    SetPixel(ix,iy, TMath::Min( GetPixel(ix,iy), im.GetPixel(ix,iy)) );
  }
}

//______________________________________________________________________________
void DMRImage::Max( const DMRImage &im )
{
  //assumed equal images
  for(int ix=0; ix<cols; ix++)
    for(int iy=0; iy<rows; iy++)
  {
    SetPixel(ix,iy, TMath::Max( GetPixel(ix,iy), im.GetPixel(ix,iy)) );
  }
}

//______________________________________________________________________________
/*bool DMRImage::ReadImage( const char *file )
{
  TASImage a;
  a.ReadImage(file);
  if(a.IsValid())
  {
    SetImage( a.GetWidth(), a.GetHeight() );
    for (int ic=0; ic<cols; ic++) 
      for (int ir=0; ir<rows; ir++) 
    {
      //SetPixel(ic,ir, )
    }
    return true;
  }
  return false;
  }*/

//______________________________________________________________________________
TH2F *DMRImage::GetHist2(int invert) const
{
  TH2F *hist=0;
  if(eImg)
  {
    const Byte_t *buf = GetImage();
    hist = new TH2F(Form("img"),"img",
                    cols, 0, cols, 
                    rows, 0, rows);
    int i=0;
    int pix;
    for (int icl=0; icl<cols; icl++) {
      for (int ir=0; ir<rows; ir++) {
        i = ir*cols + icl;
        pix = (int)(buf[i]);
        if(invert) pix = 255-pix;
        hist->Fill( icl+0.5, ir+0.5, pix );
      }
    }
  }
  return hist;
}

//______________________________________________________________________________
void DMRImage::CalculateImageStatistics()
{
  npx=0;
  pxvol=0;
  for( int i=0; i<cols; i++ )
    for( int j=0; j<rows; j++ )
  {
    Byte_t pix = GetPixel(i,j);
    if(pix)
    {
      npx++;
      pxvol++;
    }
  }
  printf("stat: npx=%d \n",npx);
}


//______________________________________________________________________________
void DMRImage::Print()
{
  printf("image: %dx%d @pol(%d)=%.1f\n",cols,rows,ipol,pol);
  printf("npx=%d pxvol=%.1f\n",npx,pxvol);
  if(eImg) printf("array size =%d\n",eImg->GetSize());
}

//______________________________________________________________________________
DMRImageCl::DMRImageCl():DMRImage()
{
  id=0;
  x=y=0;
  icl=-1;
  ifr=-1;
};

//______________________________________________________________________________
DMRImageCl* DMRImageCl::ExtractFragment( int x0, int y0, int dx, int dy )
{
  DMRImageCl *imcl = new DMRImageCl();
  DMRImage::ExtractFragment( *(imcl), x0,y0,dx,dy);
  imcl->id   = id;
  imcl->ifr  = ifr;
  imcl->icl  = icl;
  imcl->x   = x+x0-dx;
  imcl->y   = y+y0-dy;
  return imcl;
}

//______________________________________________________________________________
TH2F *DMRImageCl::GetHist2(int invert) const
{
  TH2F *h = ((DMRImage*)this)->GetHist2(invert);
  if(h)
  {
    h->SetTitle(Form("cl %d in frame %d at xy: %d %d",icl,ifr,x,y));
    h->SetName(Form("cl%df%d",icl,ifr));
  }
  return h;
}

//______________________________________________________________________________
DMRFrame::DMRFrame():DMRImage()
{
  id=uid=0;
  x=y=z=pol=0;
  ipol=0;
  time=0;
  ncl=0;
};
//______________________________________________________________________________
TH2F *DMRFrame::GetHist2(int invert) const
{
  TH2F *h = ((DMRImage*)this)->GetHist2(invert);
  if(h){
    h->SetTitle(Form("frame %d at xyz %.2f %.2f %.2f",id,x,y,z));
    h->SetName(Form("fr%d",id));
  }
  return h;
}

//______________________________________________________________________________
void DMRFrame::AddIM2FR( const DMRImageCl &imcl )
{
  for( int i=0; i<imcl.cols; i++ )
    for( int j=0; j<imcl.rows; j++ )
  {
    int x = imcl.x + i;
    int y = imcl.y + j;
    if( GetPixel(x,y)<1 ) SetPixel(x,y, imcl.GetPixel(i,j) );
  }
}

//______________________________________________________________________________
DMRImageCl* DMRFrame::ExtractFragment( int x0, int y0, int dx, int dy)
{
  DMRImageCl *imcl = new DMRImageCl();
  DMRImage::ExtractFragment( *imcl, x0,y0,dx,dy);
  
  imcl->ifr  = id;
  imcl->pol  = pol;
  imcl->ipol = ipol;
  imcl->x    = x0-dx;
  imcl->y    = y0-dy;
  return imcl;
}

//______________________________________________________________________________
DMRFrameRaw::DMRFrameRaw():DMRFrame()
{
  ventr=vid=aid=-1;
  col=row=side=-1;
};
