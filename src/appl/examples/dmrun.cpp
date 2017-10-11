//-- Author :  Valeri Tioukov   19/06/2016
// Test of DMR

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include <TRint.h>

#include "DMRLog.h"
#include "DMRRun.h"
#include "DMRRunHeader.h"

const Byte_t buf[]=
{
  1,  2, 3,
  4, 10, 6,
  7, 12, 9,
  1,  3, 1 
};

//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
//  if (argc < 2)   { print_help_message();  return 0; }

  DMRRun r("run.dm.root","RECREATE");
  r.GetHeader()->id=10;
  
  DMRView *v = r.GetView();

  for( int iv=0; iv<10; iv++ )         // cycle by views
  {

    v->HD->vid = iv;                     // fill view header information here
    v->HD->x   = 100*iv;
    v->HD->y   = 0;
    v->HD->aff.Rotate(0.01*iv);
    printf("%s\n",v->HD->aff.AsString());
    v->HD->zlayers.Set(5);
    for(int ila=0; ila<5; ila++) v->HD->zlayers.SetAt( ila*100., ila );

    for( int icl=0; icl<200; icl++ )            // fill clusters
    {
      DMRCluster *cl    = v->AddCluster();
    }

    for( int igr=0; igr<100; igr++ )            // fill grains
    {
      DMRGrain *gr      = v->AddGrain();
    }

    for( int imt=0; imt<50; imt++ )            // fill microtracks
    {
      DMRMicrotrack *mt = v->AddMicrotrack();
    }

    for( int iim=0; iim<40; iim++ )            // fill cluster images
    {
      DMRImageCl *im = v->AddImageCl();
      im->icl = 10*iim;
      im->ifr = iv;
      im->SetImage(3,4,buf);
    }

    for( int ifr=0; ifr<32; ifr++ )            // fill frames
    {
      DMRFrame *fr = v->AddFrame();
      fr->z = 10*ifr;
      fr->SetImage(3,4,buf);
    }

    r.AddView();   // here the contents of the view is stored into tree and view container "v" is cleaned
  }
  r.AddTextFile("check_image.C","testfile");
  r.Close();
}

