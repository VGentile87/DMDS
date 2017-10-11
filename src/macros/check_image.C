void check_image(const char *file="run.dm.root", int entry=5 )
{
  DMRRun  *r = new DMRRun(file);
  DMRView *v = r->GetView();
  r->GetEntry(entry,1,1,1,1,1,1);

  TCanvas *c = new TCanvas();
  c->Divide(2,2);

  c->cd(1);
  v->GetFR(1)->GetHist2()->Draw("colz");
  c->cd(2);
  v->GetFR(2)->GetHist2()->Draw("colz");

  c->cd(3);
  v->GetIM(1)->GetHist2()->Draw("colz");
  c->cd(4);
  v->GetIM(2)->GetHist2()->Draw("colz");
}