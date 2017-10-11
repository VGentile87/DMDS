#ifndef _DMRRUNHEADER_H_
#define _DMRRUNHEADER_H_

#include <TObject.h>
#include <TString.h>
#include <TDatime.h>

class DMRRunHeader : public TObject
{
  public:

    // General run information:
    Int_t     id;               // RUN id
    Int_t     flags[20];        // flags
    TDatime   startTime;        // Date and Time of run starting
    Double_t  startTimeSys;     // system start time in ms
    Int_t     nareas;           // total number of areas (fragments) in the run
    TString   comment;          //  Comment to define this scanning session

    // Scanned sample information:
    Int_t      plateID;         // plate ID
    Float_t    hUp;             // Up side   | nominal emulsion thickness before shrinkage (on the beam)
    Float_t    hBase;           // base thickness
    Float_t    hDown;           // Down side |  (normally should be the same)
    Float_t    shrinkageUp;     // srinkage of the Up side (measured in the test run)
    Float_t    shrinkageDown;   // srinkage of the Down side (measured in the test run)

    // Scanned system information:
    Int_t      npixX;           // number of pixels along X (number of columns)
    Int_t      npixY;           // number of pixels along Y (number of rows)
    Float_t    pixX;            // pixel/micron  X
    Float_t    pixY;            // pixel/micron  Y

  public:
    DMRRunHeader() { Set0(); }
    virtual ~DMRRunHeader(){}
    
    void SetMatrix(int nx, int ny, float pixx, float pixy) { npixX=nx; npixY=ny; pixX=pixx; pixY=pixy; }
    void Set0();
    void Print();
    
ClassDef(DMRRunHeader,1);
};

#endif
