#include <float.h>

using namespace TMath;

DMRRun  *run=0;
DMRView *v=0;

//void dcl(const char *file="/opera/ana/export/NEWS/C100keV/rep_pol/dm_grains.dm.root")
void dcl(const char *file="/mnt/data/NEWS/Samples_NEWS_8g/FN085GF_01_1month/set0-124/clust.dm.root")
{
  run = new DMRRun(file);
  run->GetHeader()->Print();
  run->SetFixEncoderFaults(1);
  v = run->GetView();
  gStyle->SetOptStat("n"); 
  grpolN2(109,100,10);
}

void grpolN2( int iv, int icl, int dr=15)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  int ncp=0;
  float xcp[100], ycp[100], pcp[100];
  float xcpnm[100], ycpnm[100];
  
  TH2F *hsum=0;
  DMRImage imin; imin.SetImage( 2*dr+1, 2*dr+1 );
  DMRImage imax; imax.SetImage( 2*dr+1, 2*dr+1 );
  
  TCanvas *cnv = new TCanvas("cimg","cimg",1000,750);
  cnv->Divide(4,3);
  for(int i=0; i<8; i++)
  {
    DMRImageCl *im    = run->GetCLIM(  iv,icl,i,dr );
    if(im)
    {
      TObjArray  *acl = run->GetCLCLs( iv,icl,i,dr );  //clusters
      cnv->cd(i+1)->SetGrid();
      TH2F *h = drawimcls( im,acl );
      if(h)
      {
        if(!hsum) 
        {
          hsum = (TH2F*)(h->Clone("hsum"));
          imin.Max(*im);
          imax.Max(*im);
        }
        else 
        {
          hsum->Add(h);
          imin.Min(*im);
          imax.Max(*im);
        }
      }
        
      int ncl= acl->GetEntries();
      for(int ic=0; ic<ncl; ic++)
      {
        DMRCluster *c = (DMRCluster*)(acl->At(ic));
        if( c && ncp<100 )
        {
          xcp[ncp] = c->x*pixX*1000;
          ycp[ncp] = c->y*pixY*1000;
          pcp[ncp] = im->pol;
          ncp++;
        }
      }
    }
  }
  
  float xlim[2]={FLT_MAX,-FLT_MAX};
  float ylim[2]={FLT_MAX,-FLT_MAX};
  for(int ic=0; ic<ncp; ic++)
  {
    xlim[1] = Max(xcp[ic],xlim[1]);
    ylim[1] = Max(ycp[ic],ylim[1]);
    xlim[0] = Min(xcp[ic],xlim[0]);
    ylim[0] = Min(ycp[ic],ylim[0]);
  }
  
  for(int ic=0; ic<ncp; ic++)
  {
    xcp[ic] -= xlim[0];
    ycp[ic] -= ylim[0];
  }

  cnv->cd(9)->SetGrid();
  TGraph *gxp = new TGraph(ncp,pcp,xcp);
  gxp->SetMarkerStyle(21);
  gxp->SetTitle("x vs pol [nm]");
  gxp->Draw("ACP");
  cnv->cd(10)->SetGrid();
  TGraph *gyp = new TGraph(ncp,pcp,ycp);
  gyp->SetMarkerStyle(21);
  gyp->SetTitle("y vs pol [nm]");
  gyp->Draw("ACP");
  cnv->cd(11)->SetGrid();
  for(int i=1;i>=0; i--)
  {
    xlim[i]-=xlim[0];
    ylim[i]-=ylim[0];
  }
  xlim[1] = ylim[1] = Max( xlim[1],ylim[1] );
  for(int i=0;i<2; i++)
  {
    xlim[i] += 0.1*(2*i-1)*xlim[1];
    ylim[i] += 0.1*(2*i-1)*ylim[1];
  }
  TGraph *glim = new TGraph(2,xlim,ylim);
  glim->SetTitle("y vs x [nm]");
  glim->Draw("AP");
  TGraph *gxy = new TGraph(ncp,xcp,ycp);
  gxy->SetMarkerStyle(21);
  gxy->Draw("CP");
  
  TH2F *hmin = imin.GetHist2(); hmin->SetName("hmin");
  TH2F *hmax = imax.GetHist2(); hmax->SetName("hmax");
  //cnv->cd(10)->SetGrid();
  //hmin->Draw("colz");
  //cnv->cd(11)->SetGrid();
  //hmax->Draw("colz");
  
  cnv->cd(12)->SetGrid();
  TH2F *hdiff = hmax->Clone("hdiff");
  hdiff->Add(hmin,-1);
  hdiff->Draw("colz");
  //hsum->Scale(1./ncp);
  //hsum->SetContour(64);
  //hsum->Draw("colz");
}

TH2F *drawimcls(DMRImageCl *im, TObjArray  *acl)
{
  TH2F *h =0;
  if(im)
  {
    h = im->GetHist2();
    h->Draw("colz");
    TText *t = new TText();
    t->SetTextSize(0.1);
    t->SetTextColor(1);
    t->DrawText(1,1,Form("%.2f",im->pol));	
  
    if(acl)
    {
      int ncl= acl->GetEntries();
      printf("ncl=%d\n",ncl);
      
      for(int ic=0; ic<ncl; ic++)
      {
        DMRCluster *c = (DMRCluster*)(acl->At(ic));
        if(c)
        {
          c->x = c->x - im->x;
          c->y = c->y - im->y;
          drawCL(c);
        }
      }
      
    }
  }
  return h;
}

void drawCL(float x0, float y0, DMRCluster *cl)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  TMarker *m = new TMarker(x0,y0,8);
  m->SetMarkerColor(cl->color);
  m->Draw();
  printf("id:ifr:img:igr:imt %d:%d:%d:%d:%d  major/minor = %.3f/%.3f = %.3f  bfc: %d\n",
         cl->ID(), cl->ifr, cl->img, cl->igr, cl->imt, cl->lx,cl->ly,cl->lx/cl->ly,  cl->color );
  float dxma = cl->lx*Cos(cl->phi)/2;
  float dyma = cl->lx*Sin(cl->phi)/2;
  TLine *linema = new TLine( x0-dxma,y0-dyma,x0+dxma,y0+dyma);
  linema->Draw();
  float dxmi = cl->ly*Cos(cl->phi+PiOver2())/2;
  float dymi = cl->ly*Sin(cl->phi+PiOver2())/2;
  TLine *linemi = new TLine( x0-dxmi,y0-dymi,x0+dxmi,y0+dymi);
  linemi->SetLineColor(kWhite);
  linemi->Draw();
  if(cl->color>0) 
  {
    TText *t = new TText();
    t->SetTextSize(0.025);
    t->DrawText(x0+2,y0+1,Form("%d %.2f",cl->igr, cl->lx/cl->ly));
  }
}


void GetView(int entry)
{
  run->GetEntry(entry,1,1,1,1,1,1);
  //v->AddIM2FR(run->GetHeader()->npixX, run->GetHeader()->npixY);
}


void draw8(TObjArray &imarr)
{
  int nim = imarr.GetEntries();
  TCanvas *cnv = new TCanvas("cimg","cimg",1000,750);
  cnv->Divide(4,3);
  for(int i=0; i<Min(8,nim); i++) 
  {
    DMRImageCl *im = (DMRImageCl*)(imarr.At(i));
    TH2F *h = im->GetHist2();
    //h->SetMaximum(70);
    //h->SetMinimum(25);
    cnv->cd(i+1)->SetGrid();
    h->Draw("colz");
    TText *t = new TText();
    t->SetTextSize(0.1);
    t->SetTextColor(0);
    t->DrawText(1,1,Form("%.2f",im->pol));
  }
}


void frr(int ifr)
{
  //DMRFrameRaw        *fr = run->GetFrameRaw( ifr );
  DMRFrameRaw        *fr = run->GetFrameRaw( "ventr==0&&iz==17&&ipol==0" );
  if(fr) {
    fr->Print();
    TH2F *hfr = fr->GetHist2();
    if(hfr) hfr->Draw("colz");
  }
}

void fr0(int ifr)
{
  DMRFrame        *fr = v->GetFR( ifr );
  if(fr) {
    fr->Print();
    TH2F *hfr = fr->GetHist2();
    if(hfr) hfr->Draw("colz");
  }
}

void grbf( int igr)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  int nx     = run->GetHeader()->npixX;
  int ny     = run->GetHeader()->npixY;

  DMRViewHeader   *hd = v->GetHD();
  DMRGrain        *gr = v->GetGR(igr);
  DMRCluster      *cl = v->GetCL(gr->ibfc);
  DMRImageCl      *im = v->GetIM(cl->img);
  DMRFrame        *fr = v->GetFR(cl->ifr,cl->pol);

  TCanvas *c = new TCanvas();
  TH2F *h = im->GetHist2();
  h->Draw("colz");
  cl->color=kBlack;
  float x0 = ((cl->x+hd->x)-fr->x)/pixX + nx/2 - im->x;
  float y0 = ((cl->y+hd->y)-fr->y)/pixY + ny/2 - im->y;
  drawCL(x0,y0,cl);
}

void grpol(int igr, int dr=10)
{
  int dx=dr;
  int dy=dr;
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  int nx     = run->GetHeader()->npixX;
  int ny     = run->GetHeader()->npixY;

  DMRViewHeader   *hd = v->GetHD();
  DMRGrain        *gr = v->GetGR(igr);
  DMRCluster      *cl = v->GetCL(gr->ibfc);
  DMRFrame        *frcl = v->GetFR(cl->ifr);

  TObjArray *arr = run->GetFramesRaw( v->ID(),frcl->iz );  //frame with all polarizations
  int nfr=arr->GetEntries();
  printf("nfr=%d\n",nfr);
  
  int ncp=0;
  float xcp[100], ycp[100], pcp[100];
  float xcpnm[100], ycpnm[100];

  TCanvas *cnv = new TCanvas("cimg",Form("view: %d gr: %d  bfc@frame:%d", hd->vid, igr, cl->ifr ),1000,750);
  cnv->Divide(4,3);
  for(int i=0; i<Min(9,nfr); i++) 
  {
    DMRFrame   *fr = (DMRFrame*)(arr->At(i));
    float x0 = ((cl->x+hd->x)-fr->x)/pixX + nx/2;
    float y0 = ((cl->y+hd->y)-fr->y)/pixY + ny/2;
    DMRImageCl *im = fr->ExtractFragment(x0,y0,dx,dy);
    TH2F *h = im->GetHist2();
    h->SetMaximum(70);
    h->SetMinimum(25);
    cnv->cd(i+1)->SetGrid();
    h->Draw("colz");
    TText *t = new TText();
    t->SetTextSize(0.1);
    t->SetTextColor(0);
    t->DrawText(1,1,Form("%.2f",fr->pol));
    TObjArray *acl = v->GetClusters( cl->x, cl->y, Abs(dx*pixX), Abs(dy*pixY), fr->ID(), fr->pol);
    int ncl=acl->GetEntries();
    printf("ncl=%d\n",ncl);
    for(int ic=0; ic<ncl; ic++)
    {
      DMRCluster *c = (DMRCluster*)(acl->At(ic));
    //if(c->ID()==gr->ibfc)  cl->color=kBlack;
      float x0 = ((c->x+hd->x)-fr->x)/pixX + nx/2 - im->x;
      float y0 = ((c->y+hd->y)-fr->y)/pixY + ny/2 - im->y;
      drawCL(x0,y0,c);
      if(ncp<100)
      {
        xcp[ncp] = x0;
        ycp[ncp] = y0;
        pcp[ncp] = fr->pol;
        ncp++;
      }
    }
    //cnv->Update();
    //gSystem->Sleep(100);
  }
  
  cnv->cd(9)->SetGrid();
  TGraph *gxp = new TGraph(ncp,pcp,xcp);
  gxp->SetMarkerStyle(21);
  gxp->SetTitle("x vs pol [pix]");
  gxp->Draw("ACP");
  cnv->cd(10)->SetGrid();
  TGraph *gyp = new TGraph(ncp,pcp,ycp);
  gyp->SetMarkerStyle(21);
  gyp->SetTitle("y vs pol [pix]");
  gyp->Draw("ACP");
  cnv->cd(11)->SetGrid();
  TGraph *gxy = new TGraph(ncp,xcp,ycp);
  gxy->SetMarkerStyle(21);
  gxy->SetTitle("y vs x [pix]");
  gxy->Draw("ACP");
  
  float xcpnm0=0;
  float ycpnm0=0;
  for(int i=0; i<ncp; i++) 
  {
    xcpnm[i]=xcp[i]*pixX*1000;
    ycpnm[i]=ycp[i]*pixY*1000;
    xcpnm0+=xcpnm[i];
    ycpnm0+=ycpnm[i];
  }
  xcpnm0/=ncp;
  ycpnm0/=ncp;
  for(int i=0; i<ncp; i++) 
  {
    xcpnm[i] -= xcpnm0;
    ycpnm[i] -= ycpnm0;
  }
  cnv->cd(12)->SetGrid();
  TGraph *gxynm = new TGraph(ncp,xcpnm,ycpnm);
  gxynm->SetMarkerStyle(21);
  gxynm->SetTitle("y vs x (nm)");
  gxynm->Draw("ACP");
  

}

void gr( int igr)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  int nx     = run->GetHeader()->npixX;
  int ny     = run->GetHeader()->npixY;

  DMRViewHeader   *hd = v->GetHD();
  DMRGrain        *gr = v->GetGR(igr);

  TObjArray *arr = v->GetCLofGR(igr);
  int ncl=arr->GetEntries();
  printf("ncl=%d\n",ncl);
  
  TCanvas *c = new TCanvas("c","c",600,600);
  c->Divide(3,3);
  
  for(int i=0; i<Min(9,ncl); i++) 
  {
    c->cd(i+1);
    DMRCluster      *cl = (DMRCluster*)arr->At(i);
    DMRImageCl      *im = v->GetIM(cl->img);
    DMRFrame        *fr = v->GetFR(cl->ifr,cl->pol);
    TH2F *h = im->GetHist2();
    h->Draw("colz");
    if(cl->ID()==gr->ibfc)  cl->color=kBlack;
    float x0 = ((cl->x+hd->x)-fr->x)/pixX + nx/2 - im->x;
    float y0 = ((cl->y+hd->y)-fr->y)/pixY + ny/2 - im->y;
    drawCL(x0,y0,cl);
  }
}


void drawCL( DMRCluster *cl)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  TMarker *m = new TMarker(cl->x,cl->y,8);
  m->SetMarkerColor(cl->color);
  m->Draw();
  printf("id:ifr:img:igr:imt %d:%d:%d:%d:%d  major/minor = %.3f/%.3f = %.3f\n",
         cl->ID(), cl->ifr, cl->img, cl->igr, cl->imt, cl->lx,cl->ly,cl->lx/cl->ly );
  float dxma = cl->lx*Cos(cl->phi)/2/pixX;
  float dyma = cl->lx*Sin(cl->phi)/2/pixY;
  TLine *linema = new TLine( cl->x-dxma,cl->y-dyma,cl->x+dxma,cl->y+dyma);
  linema->Draw();
  float dxmi = cl->ly*Cos(cl->phi+PiOver2())/2/pixX;
  float dymi = cl->ly*Sin(cl->phi+PiOver2())/2/pixY;
  TLine *linemi = new TLine( cl->x-dxmi,cl->y-dymi,cl->x+dxmi,cl->y+dymi);
  linemi->SetLineColor(kWhite);
  linemi->Draw();
  if(cl->color>0) 
  {
    TText *t = new TText();
    t->SetTextSize(0.025);
    t->DrawText(cl->x+2,cl->y+1,Form("%d %.2f",cl->igr, cl->lx/cl->ly));
  }
}

void fr(int ifr, int pol)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  int nx     = run->GetHeader()->npixX;
  int ny     = run->GetHeader()->npixY;
  DMRViewHeader   *hd = v->GetHD();
  
  DMRFrame        *fr = v->GetFR( ifr, pol );
  
  //fr->RemovePixelNoise(3);
  TH2F *hfr = fr->GetHist2();
  //hfr->Smooth();
  hfr->Draw("colz");
  /*
  int ncl = v->CL->GetEntries();
  for(int i=0; i<ncl; i++)
  {
  DMRCluster *cl = v->GetCL(i);
  if(v->GetGR(cl->igr)->ibfc==cl->ID()) cl->color=kBlack;
  if( cl && cl->ifr==ifr && v->SamePol(cl->pol,fr->pol) )
  {
  float x0 = ((cl->x+hd->x)-fr->x)/pixX + nx/2;
  float y0 = ((cl->y+hd->y)-fr->y)/pixY + ny/2;
  drawCL(x0, y0, cl);
}
}
  */
}
