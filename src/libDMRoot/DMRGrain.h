#ifndef _DMRGRAIN_H_
#define _DMRGRAIN_H_

#include <TObject.h>

class DMRGrain : public TObject
{
  friend class DMRView;
  private:
    UInt_t id;
public:
	enum {
		gf_Merged = 0x01, // grain from a set of merged images
		gf_Extra = 0x02   // grain from a set if extra images
	};
  UInt_t icp;     // copy index (in case of grain duplication (0-default value)
  UInt_t flags;
	Float_t x,y,z;	// coordinates
	UInt_t ncl;			// number of clusters
	UInt_t npx;			// total number of pixels (summed over all contained clusters)
	UInt_t vol;			// total volume (summed over all contained clusters)
	Float_t lx,ly,lz,phi,theta; // ellipsoidal fit: 3 axes, 2 slopes
	Int_t frFirst,frLast;	// indices of first and last frames
  Float_t pol;        // polarization angle
  Int_t ipol;					// polarization angle index

	Int_t ibfc;	// index of the best focused cluater
	Int_t imt;	// index of the microtrack owing this grain
	Int_t ipt;	// index of the plasmontrack owing this grain

	Int_t is_nt; // 1 if this grain is a nanotrack
  public:
    DMRGrain();
    virtual ~DMRGrain(){}
    
    Int_t ID()const{return id;}

ClassDef(DMRGrain,1);
};

#endif
