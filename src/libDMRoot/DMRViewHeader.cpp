//-- Authors :  AA,VT   19/06/2016
#include "DMRViewHeader.h"

ClassImp(DMRViewHeader)

void DMRViewHeader::Set0()
{
  vid=aid=side-1;
  flag=0;
  col=row=0;
  x=y=z=0;
  xmin=xmax=ymin=ymax=zmin=zmax=0;
  rmax=-1;
  ncl=ngr=nmt=nim=nfr=0;
  track=event=-1;
  time=0;
}
