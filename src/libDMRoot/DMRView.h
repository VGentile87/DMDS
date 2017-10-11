#ifndef _DMRVIEW_H_
#define _DMRVIEW_H_

#include <TObject.h>
#include <TClonesArray.h>

#include "DMRViewHeader.h"
#include "DMRCluster.h"
#include "DMRGrain.h"
#include "DMRMicrotrack.h"
#include "DMRImage.h"
using namespace TMath;

class DMRView : public TObject
{
  friend class DMRRun;
  public:

  DMRViewHeader    *HD;    // View header
  TClonesArray     *CL;    // array of Clusters
  TClonesArray     *GR;    // array of Grains
  TClonesArray     *MT;    // array of MicroTracks
  TClonesArray     *IM;    // array of cluster images
  TClonesArray     *FR;    // array of Frames
  
  private:
  bool  redHD; //!
  bool  redCL; //!
  bool  redGR; //!
  bool  redMT; //!
  bool  redIM; //!
  bool  redFR; //!
  
  public:
    DMRView();
    virtual  ~DMRView();
    
    void Clear();

    DMRCluster    *AddCluster();
    DMRGrain      *AddGrain();
    DMRMicrotrack *AddMicrotrack();
    DMRImageCl    *AddImageCl();
    DMRFrame      *AddFrame();

    DMRViewHeader *GetHD()      const { return HD; }
    DMRCluster    *GetCL(int i) const { return (DMRCluster*)CL->At(i); }
    DMRGrain      *GetGR(int i) const { return (DMRGrain*)GR->At(i); }
    DMRMicrotrack *GetMT(int i) const { return (DMRMicrotrack*)MT->At(i); }
    DMRImageCl    *GetIM(int i) const { return (DMRImageCl*)IM->At(i); }
    DMRFrame      *GetFR(int i) const { return (DMRFrame*)FR->At(i); }
    DMRFrame      *GetFR(Int_t iz, int ipol) const;
    
    bool        SamePol(float pol1, float pol2) const {return Abs(pol1-pol2)<2? true: false;}
    TObjArray  *GetCLofGR(int igr);
    TObjArray  *GetFrames(Int_t ifr);
    TObjArray  *GetClusters(float x0, float y0, float dx, float dy, Int_t ifr, int ipol);
    void        AddIM2FR(int ncol, int nrow);
    DMRImageCl *GetImageCl(const DMRCluster &cl, int dx, int dy);
    void FixEncoderFaults();

ClassDef(DMRView,1);
};

#endif
