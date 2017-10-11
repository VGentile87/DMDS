#ifndef _DMRCLUSTER_H_
#define _DMRCLUSTER_H_

#include <TObject.h>

class DMRCluster : public TObject
{
  friend class DMRView;
  private:
    UInt_t id;          // clusters counter inside this view
public:
	enum {
		cf_Merged = 0x01, // cluster from a merged image
		cf_Extra = 0x02   // cluster from an extra image
	};

  UInt_t id_f;        // cluster id inside frame ifr
  UInt_t icp;         // copy index (in case of cluster duplication (0-default value)
  UInt_t flags;       //
  Float_t x,y,z;			// coordinates (in view RS)
	Float_t lx,ly,phi;	// elliptical fit: major, minor, direction
	Float_t gh,gb,gq;		// gaussian fit: height, bg, quality
	UInt_t  npx;        // number of pixels
	UInt_t  vol;				// total volume
	Float_t pol;				// polarization angle
  Int_t   ipol;       // polarization angle index
  Int_t   color;      // color index if available (default=0)

	Int_t img;					// index of corresponding image
	Int_t ifr;					// owing frame index
	Int_t igr;					// owing grain index
	Int_t imt;					// owing microtrack index

	Int_t is_nt;				// 1 if this cluster is a nanotrack
  public:
    DMRCluster();
    virtual ~DMRCluster(){}

    Int_t ID()const {return id;}
    void ToPixels(float pixX, float pixY, int nx, int ny);

 ClassDef(DMRCluster,1);
};

#endif
