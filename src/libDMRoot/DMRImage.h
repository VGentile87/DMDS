#ifndef _DMRIMAGE_H_
#define _DMRIMAGE_H_

#include <TObject.h>
#include <TArrayC.h>
class TH2F;

//__________________________________________________________________________________
class DMRImage : public TObject
{
  public:
	enum {
		if_Merged = 0x01,             // image was obtained by merging together different polarizations
		if_Extra = 0x02               // image is an extra polarization angle intended for validation
	};

    Int_t    cols,rows;           // image size
    Int_t    sensor;              // sensor id (important in case of multycamera systems)
    Int_t    color;               // color index if available (default=0)
    Float_t  pol;                 // polarization angle
    Int_t    ipol;                // polarization angle index
    Int_t    npx;                 // number of pixels above threshold
    Double_t pxvol;               // integral volume of pixels above the threshold
    Float_t  ll;                  // light level (volts or au)
    Float_t  thres;               // threshold for the clasterization
    UInt_t flags;
  
  private:
    TArrayC  *eImg;               // image buffer

  public:
    DMRImage();
    virtual ~DMRImage();
    
    const Byte_t* GetImage()const
    {
      return eImg ? (const Byte_t*)eImg->GetArray() : 0 ;
    };
    Byte_t* GetImage()
    {
      return eImg ? (Byte_t*)eImg->GetArray() : 0 ;
    };

    Int_t Npix() const { return cols*rows; }
    void Print();
    void SetImage( Int_t cols, Int_t rows, const Byte_t* img);
    void SetImage( Int_t cols, Int_t rows );
    TH2F *GetHist2(int invert=0) const;
    Int_t I(int x, int y) const { if(x>=0&&x<cols&&y>=0&&y<rows) return y*cols+x; return -1; }
    Byte_t GetPixel(int i) const { return GetImage()[i]; }
    Byte_t GetPixel(int x, int y) const { if(I(x,y)>0&&GetImage()) return GetPixel(I(x,y)); return 0; }
    void   SetPixel(int x, int y, Byte_t pix) { if(eImg&&x>0&&x<cols&&y>0&&y<rows) GetImage()[y*cols+x]=pix; }
    void RemovePixelNoise( int delta );
    bool ReadImage(const char *file);
    //virtual DMRImage *ExtractFragment( int x0, int y0, int dx, int dy );
    void CalculateImageStatistics();
    void Min( const DMRImage &im );
    void Max( const DMRImage &im );
    
  protected:
    void      ExtractFragment( DMRImage &im, int x0, int y0, int dx, int dy );
 
  private:
    explicit DMRImage(const DMRImage&);
    DMRImage& operator =(const DMRImage&);
    void Set0();

    ClassDef(DMRImage,2); // generic image container
};

//__________________________________________________________________________________
class DMRImageCl : public DMRImage
{
  friend class DMRView;
  private:
    UInt_t id;                   // imagecl id (entry number in IM array)
  public:
    Int_t x,y;                   // lowest x,y in pixels of this image fragment inside frame
    Int_t icl;                   // cluster id
    Int_t ifr;                   // frame id inside this view (entry number in frames array)

  public:
    DMRImageCl();
    virtual ~DMRImageCl(){}
    Int_t ID()const {return id;}
    TH2F *GetHist2(int invert=0) const;
    DMRImageCl *ExtractFragment( int x0, int y0, int dx, int dy );

    ClassDef(DMRImageCl,1); // container for one image fragment (associated with cluster)
};

//__________________________________________________________________________________
class DMRFrame : public DMRImage
{
  friend class DMRView;
  private:
    UInt_t id;                    // frame id inside view (entry number in RF array)
  public:
    UInt_t   uid;                 // unique frame id inside run
    Int_t    ix,iy,iz;            // iz-step index, ix,iy - not used yet
    Float_t  x,y,z;               // coordiantes of the frame plateRS
    Double_t time;                // relative time in ms
    Int_t    ncl;                 // number of clusters in the frame
    
  public:
    DMRFrame();
    virtual ~DMRFrame(){}
    TH2F *GetHist2(int invert=0) const;
    Int_t ID()const {return id;}
    void AddIM2FR( const DMRImageCl &imcl );
    DMRImageCl *ExtractFragment( int x0, int y0, int dx, int dy);
    
    ClassDef(DMRFrame,1); // container for one camera's frame
};

//__________________________________________________________________________________
class DMRFrameRaw : public DMRFrame
{
  public:
    Int_t    ventr;       // view entry nomber in Vdmr
    Int_t    vid;         // view counter inside one area
    Int_t    aid;         // area id
    Int_t    col,row;     // view position inside an area
    Int_t    side;        // 0-down,1-up

  public:
    DMRFrameRaw();
    virtual ~DMRFrameRaw(){}
    
    ClassDef(DMRFrameRaw,1); // container for one camera's frame for storage into the independent tree
};
//__________________________________________________________________________________

#endif
