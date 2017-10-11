#ifndef _DMRVIEWHEADER_H_
#define _DMRVIEWHEADER_H_

#include <TObject.h>
#include <TArrayF.h>
#include "DMRAffine2D.h"

class DMRViewHeader : public TObject
{
  friend class DMRRun;
  private:
    UInt_t     id;         // view id (entry number in the tree)
  public:
    Int_t    vid;         // view counter inside one area
    Int_t    aid;         // area id
    Int_t    col,row;     // view position inside an area
    Int_t    flag;        // flag
    Int_t    side;        // 0-down,1-up
    Float_t  x,y,z;       // center of the view
    Float_t  rmax;        // radial limit around view center (default -1: not used)
    Float_t  xmin,xmax;   // view limits
    Float_t  ymin,ymax;   //
    Float_t  zmin,zmax;   //
    TArrayF  zlayers;     // in case of n layers n+1 layer limits
    TArrayF  polsteps;    // polstep[ipol] - polarization angle
    Int_t    ncl;         // number of stored clusters
    Int_t    ngr;         // number of stored grains
    Int_t    nmt;         // number of stored microtracks
    Int_t    nim;         // number of stored images
    Int_t    nfr;         // number of stored frames
    Int_t    track;       // prediction id in case of point scan
    Int_t    event;       // event id in case of point scan
    Double_t time;        // relative acquisition time in ms
    DMRAffine2D aff;      // affine transformation from viewRS to plateRS
    
  public:
    DMRViewHeader() { Set0(); }
    virtual ~DMRViewHeader(){}
    
    Int_t ID()const{return id;}
    void Set0();
    
ClassDef(DMRViewHeader,1);
};

#endif
