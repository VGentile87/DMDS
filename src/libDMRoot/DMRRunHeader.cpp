//-- Author :  VT   19/06/2016
#include "DMRRunHeader.h"
ClassImp(DMRRunHeader)

void DMRRunHeader::Set0()
{
  id=0;
  for(int i=0;i<20; i++) flags[i]=0;
  startTimeSys=0;
  nareas=0;
  
  plateID=0;
  hUp=hBase=hDown=0;
  shrinkageUp=shrinkageDown=1;
  
  npixX=npixY=0;
  pixX=pixY=0;
}

void DMRRunHeader::Print()
{
  printf("Run %d\n",id);
  printf("Matrix: %d x %d  with pixel: %f x %f\n",npixX,npixY,pixX,pixY);
}
