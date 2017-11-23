#ifndef DMRTextFile_H_
#define DMRTextFile_H_

#include <string>
#include <TArrayC.h>
#include <TObject.h>

class DMRTextFile : public TObject
{
  private:
    std::string originalname;
    std::string buffer;
    size_t size;
  public:
    DMRTextFile();
    virtual  ~DMRTextFile();
    Int_t ReadFile(const char *file);
    Int_t WriteFile(const char *file=0);
    void Print();

ClassDef(DMRTextFile,1);
};

#endif
