//-- Authors :  AA,VT   19/06/2016
#include "DMRCluster.h"
ClassImp(DMRCluster)

DMRCluster::DMRCluster()
{
  id=id_f=0;
  icp=0;
  flags=0;
  x=y=z=0;
  lx=ly=phi=0;
  gh=gb=gq=0;
  npx=0;
  vol=0;
  pol=0;
  ipol=0;
  color=0;
  img=ifr=igr=imt=0;
  is_nt=0;
}

void DMRCluster::ToPixels(float pixX, float pixY, int nx, int ny)
{
  x = x/pixX + nx;
  y = y/pixY + ny;
  lx/=pixX;
  ly/=pixY;
}
