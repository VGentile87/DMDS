#ifndef _DMRFIDUCIAL_H_
#define _DMRFIDUCIAL_H_

#include <TObject.h>
#include <TObjArray.h>
#include "DMRAffine2D.h"

class DMRFiducialMark : public TObject
{
  friend class DMRMarksSet;
  public:
    Int_t     id;            // mark id
    Int_t     type;          // type of mark (x-ray, lateral, blob, etc)
    Float_t   x_abs,y_abs;   // absolute (nominal) coords
    Float_t   x_msr,y_msr;   // measured coords
    Bool_t    is_found;      // true if mark was found
  private:
    Int_t      cnt;          // counter (inside marks set)
  public:
    DMRFiducialMark(){}
    virtual ~DMRFiducialMark(){}
    int Cnt() {return cnt;}
    
    ClassDef(DMRFiducialMark,1)
};

//______________________________________________________________________________
class DMRMarksSet : public TObject 
{
  private:
    TObjArray eMarks;
    
  public: 
    DMRMarksSet();
    virtual ~DMRMarksSet(){}
    
    DMRFiducialMark *NewMark();
    DMRFiducialMark *GetMark(int i) const {return (DMRFiducialMark *)eMarks.At(i); }
    int              N() const {return eMarks.GetEntries();}
    
    DMRAffine2D    *Abs2Stage(bool invert=false)    const;
    
    ClassDef(DMRMarksSet,1)  //
};

#endif
