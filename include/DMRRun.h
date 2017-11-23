#ifndef _DMRRUN_H_
#define _DMRRUN_H_

#include "DMRView.h"

#include <TObject.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>

class DMRRunHeader;

struct AlignmentParView
{
  Int_t view1;  // 1-st view
  Int_t view2;  // 2-d view
  Int_t area1;
  Int_t area2;
  Int_t side1;
  Int_t side2;
  Float_t dx;   // found offset
  Float_t dy;   // found offset
  Float_t dz;   // found offset
  Int_t n1tot;  // total number of grains in the view
  Int_t n2tot;
  Int_t n1;     // number of grains in intersection area
  Int_t n2;
  Int_t nsg;   // peak height
  Int_t nbg;   // bg level (random coinsidences)
  Int_t flag;  // enum Flags {f_Applied = 0x01,f_Pin = 0x02,f_Found = 0x04}; f_Pin: view2 is a pinned view
};

struct AlignmentParFrame
{
  Int_t frame1; // 1-st frame id
  Int_t frame2; // 2-d frame id
  Int_t view;   // view id
  Int_t area;
  Int_t side;
  Float_t dxglobal;  // global offset applied for all frames of this view (XfrStage=Xframecorrected-dxglobal-flag*dx)
  Float_t dyglobal;  // global offset applied for all frames of this view
  Float_t dx;        // found offset
  Float_t dy;        // found offset
  Float_t z1;        // z of the first frame
  Float_t z2;        // z of the second frame
  Int_t n1tot;       // total number of clusters in the frame
  Int_t n2tot;
  Int_t n1;          // number of grains in intersection area
  Int_t n2;
  Int_t nsg;         // peak height
  Int_t nbg;         // bg level (random coinsidences)
  Int_t flag;        // in first 3 bits: {f_Applied = 0x01,f_Found = 0x02,f_Recovered = 0x04};
};

class DMRRun : public TObject
{
  private:
    DMRRunHeader      *eHeader;         // run header
    
    DMRView           *eView;           // data exchange container
    TTree             *eTree;           // tree with Views
    TFile             *eFile;           // file associated with the Run
    TString            ePath;           // runs directory path
    
    TTree             *eViewAlign;      // view neighbours  alignment
    TTree             *eFrameAlign;     // frames alignment
    AlignmentParView   eVA;
    AlignmentParFrame  eFA;
    DMRViewHeader     *eVH1;            // alignment headers
    DMRViewHeader     *eVH2;            // alignment headers
    
    TTree             *eFrames;         // raw frames (images)
    DMRFrameRaw       *eFrameRaw;       // container for storage into Frames tree
  
    Bool_t             eFixEncoderFaults;
  private:
    void Init0();

  public:
    DMRRun();
    DMRRun(const char *fname, const char *opt=0);
    virtual ~DMRRun();

    void OpenExistingFile( const char *fname, const char *opt );
    void OpenNewFile( const char *fname );
    
    DMRRunHeader        *GetHeader() const { return eHeader;}
    const DMRView             *GetView()   const { return eView; }
    DMRView             *GetView();
    TTree               *GetTree()   const { return eTree; }
    void                 SetView(DMRView *view);
    void                 SetView();
    
    void                 SetFixEncoderFaults(bool dofix) {eFixEncoderFaults=dofix;}
		void								 SetMaxFileSize(Long64_t bytes_);
    
    void AddView()  { AddView(eView); }
    void AddView( DMRView *view );
    void Close();
    TFile    *GetFile(){ return eFile; }
    void AddTextFile(const char *file, const char *name);

    DMRView  *GetEntry( int entry, int ihd=1, int icl=0, int igr=0, int imt=0, int iim=0, int ifr=0 );
    
    void AddViewAlign( Int_t view1, Int_t view2, Int_t area1, Int_t area2, Int_t side1, Int_t side2,
                       Float_t dx, Float_t dy, Float_t dz, 
                       Int_t n1tot, Int_t n2tot, Int_t n1, Int_t n2, Int_t nsg, Int_t nbg, Int_t flag,
                       DMRViewHeader &vh1, DMRViewHeader &vh2 );

    void AddFrameAlign( Int_t frame1, Int_t frame2, Int_t view, Int_t area, Int_t side,
                        Float_t dxglobal, Float_t dyglobal, Float_t dx, Float_t dy, Float_t z1, Float_t z2, 
                        Int_t n1tot, Int_t n2tot, Int_t n1, Int_t n2, Int_t nsg, Int_t nbg, Int_t flag);
    
	void         AddFrameRaw();
  bool         SetFrames();
  DMRFrameRaw *GetFrameRaw()   const { return eFrameRaw; }
  DMRFrameRaw *GetFrameRaw(int entr);
	DMRFrameRaw *GetFrameRaw(const char *cut);
  TObjArray   *GetImagesCl(int ventr, int iz, float x0, float y0, int dx, int dy);
  DMRImageCl  *GetGRIM(   int iv, int igr, int ipol, int dr);
  DMRImageCl  *GetGRIMBFC(   int iv, int igr, int ipol, int dr, int x0, int y0);
  DMRImageCl  *GetCLIM(   int iv, int igr, int ipol, int dr );
  DMRImageCl  *GetCLIMBFC(   int iv, int icl, int ipol, int dr, int x0, int y0);
  TObjArray   *GetGRCLs(  int iv, int igr, int ipol, int dr );
  TObjArray   *GetCLCLs(  int iv, int icl, int ipol, int dr );

ClassDef(DMRRun,1);
};

#endif
