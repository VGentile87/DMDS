//-- Authors :  VT   04/11/2016

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "DMRTextFile.h"
ClassImp(DMRTextFile)

//______________________________________________________________________________
DMRTextFile::DMRTextFile():TObject()
{
  size=0;
}

//______________________________________________________________________________
DMRTextFile::~DMRTextFile()
{
}

//______________________________________________________________________________
Int_t DMRTextFile::ReadFile(const char *file)
{
  std::ifstream t(file);
  t.seekg(0, std::ios::end);
  size = t.tellg();
  t.seekg(0);
  buffer.reserve(size);
  t.read( &buffer[0],size);
  originalname=file;
  return size;
}

//______________________________________________________________________________
Int_t DMRTextFile::WriteFile(const char *newname)
{
  const char *n = newname?newname:originalname.c_str();
  std::ofstream out(n);
  out.write( buffer.c_str(), size);
  out.close();
  return size;
}

//______________________________________________________________________________
void DMRTextFile::Print()
{
  cout << originalname << endl;
  cout.write( buffer.c_str(), size);
}

