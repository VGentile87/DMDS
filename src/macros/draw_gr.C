using namespace TMath;

DMRRun  *run=0;
DMRView *v=0;

void draw_gr(const char *file="/mnt/data/NEWS/C100keV/rep_nop/dm_grains.dm.root")
{
  run = new DMRRun(file);
  run->GetHeader()->Print();
  v = run->GetView();
  gStyle->SetOptStat("n");
}

void GetView(int entry)
{
  run->GetEntry(entry,1,1,1,1,1,1);
  //v->AddIM2FR(run->GetHeader()->npixX, run->GetHeader()->npixY);
}

void fr0(int ifr)
{
  DMRFrame        *fr = v->GetFR( ifr );
  if(fr) {
    fr->Print();
    TH2F *hfr = fr->GetHist2();
    //if(hfr) hfr->Draw("colz");
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

  TObjArray *arr = v->GetFrames(cl->ifr);  //frame with all polarizations
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


void drawCL(float x0, float y0, DMRCluster *cl)
{
  float pixX = run->GetHeader()->pixX;
  float pixY = run->GetHeader()->pixY;
  TMarker *m = new TMarker(x0,y0,8);
  m->SetMarkerColor(cl->color);
  m->Draw();
  printf("id:ifr:img:igr:imt %d:%d:%d:%d:%d  major/minor = %.3f/%.3f = %.3f  bfc: %d\n",
         cl->ID(), cl->ifr, cl->img, cl->igr, cl->imt, cl->lx,cl->ly,cl->lx/cl->ly,  cl->color );
  float dxma = cl->lx*Cos(cl->phi)/2/pixX;
  float dyma = cl->lx*Sin(cl->phi)/2/pixY;
  TLine *linema = new TLine( x0-dxma,y0-dyma,x0+dxma,y0+dyma);
  linema->Draw();
  float dxmi = cl->ly*Cos(cl->phi+PiOver2())/2/pixX;
  float dymi = cl->ly*Sin(cl->phi+PiOver2())/2/pixY;
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
