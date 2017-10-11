#ifndef _DMRMICROTRACK_H_
#define _DMRMICROTRACK_H_

#include <TObject.h>
#include <TObjArray.h>
#include "DMRGrain.h"

class DMRMicrotrack : public TObject
{
  friend class DMRView;
  private:
    UInt_t id;
  public:
    UInt_t flags;
    UInt_t ncl;								// number of clusters
    UInt_t ngr;								// number of grains
    Int_t frFirst,frLast;			// indices of first and last frames triggered by this track
    UInt_t frTrig;						// number of triggered frames
    UInt_t npx;								// total number of pixels (summed over all grains/clusters)
    UInt_t vol;								// total volume (summed over all grains/clusters)
    Float_t limX0,limY0,limZ0;		// coordinate of one corner of the bounding box
    Float_t limX1,limY1,limZ1;		// coordinate of the opposite corner of the bounding box
    Float_t x,y,z,phi,theta;								// fitted coordinates and slopes
    Float_t sigX,sigY,sigZ,sigPhi,sigTheta;	// sigmas of fitted coordinates and slopes
    Float_t chi2;														// chi2 of the fit
    Float_t len;			// length
    Float_t score;		// score

    Int_t is_pt;			// 1 if this track is due to plasmon analysis

    TObjArray *grains; //! transient grains array (does not saved into root file)
  
  public:
    DMRMicrotrack(){}
    virtual ~DMRMicrotrack(){}
    Int_t ID()const {return id;}
    void MakeMicrotrack( const TObjArray &grains );

    ClassDef(DMRMicrotrack,2);
};

#endif
