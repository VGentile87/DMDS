void rootlogon()
{
  // DM lib path should be setted in LD_LIBRARY_PATH
  printf("\nLoad DM libs: \n");
 
  loadlib( "libDMRoot"  , "DMRView" );
 
  printf("\n\n");

  gStyle->SetPalette(1);
}

//----------------------------------------------------------
void loadlib(const char *lib, const char *key)
{
  if (!TClassTable::GetDict(key)) { 
    if(gSystem->Load(lib)) printf("\n ERROR: \n%s is NOT loaded!",lib);
    else printf(" %s",lib);
  }
}


