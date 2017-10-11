#include "TSystem.h"
#include "TROOT.h"
#include "Riostream.h"
#include <utility>
#include <vector>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include "TString.h"
#include "TUnixSystem.h"
#include "TTree.h"
#include <map>


using namespace std;

void start(){
  int s=1;
  do{
    cout << "Premi 1 per MakeClass, 2 per libDMRoot, 3 per Analisi, 0 per uscire: ";
    //cin >> s;
    if(s==1){
      gROOT->ProcessLine("gSystem->Load(\"libDMRoot\")");
      gROOT->ProcessLine("Vdmr->MakeClass(\"myData\")");
      gSystem->Exec("rm myData.C");
      gSystem->Exec("cp $DM_ROOT/src/macros/myData_v5.C .");
      gSystem->Exec("cp $DM_ROOT/src/macros/settings.mac .");

      TString dir = gSystem->UnixPathName(__FILE__);
      dir.ReplaceAll("start.C","");
      dir.ReplaceAll("/./","/");
      ifstream in;
      in.open(Form("%smyData.h",dir.Data()));

      ofstream out("MyData.h");

      int iline=0;
      vector<vector<string> > runDescription = vector<vector<string> >();
      //runDescription = new std::vector<vector<string> >;
      if (in.is_open()){
	string line;
	out << line << endl;
	while (getline (in,line)){
	  iline++;
	  istringstream iss(line);
	  string a;
	  vector<string> tmpS;
	  if(iline<16 || iline>25 && iline!=384){
	    while(!iss.eof()){     
	      iss>>a;
	      tmpS.push_back(a);
	      out << a << " ";
	      
	  }
	    runDescription.push_back(tmpS);
	    tmpS.clear();
	    out << endl;
	  }
	  if(iline==16){
	    out << "#include <TObject.h>" << endl;
	    out <<"#include <TClonesArray.h>" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRViewHeader.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRAffine2D.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRCluster.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRGrain.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRMicrotrack.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRImage.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRImage.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRImage.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRRun.h\"" << endl;
	    out <<"#include \"/home/vale/DMDS/dm2root_v7/include/DMRRunHeader.h\"" << endl;
	  }
	  if(iline==384) out << "myData::myData(TTree *tree) : fChain(0)" << endl;
	}
	out.close();
	in.close();
      }
      gSystem->Exec("rm myData.h");
      gSystem->Exec("mv MyData.h myData.h");

      //gSystem->Exec("");
      //gROOT->ProcessLine(".x myData_v5.C++");
      //gROOT->ProcessLine("myrun()");
      //runDescription.clear();
      //delete runDescription;
      gROOT->ProcessLine(".q");
  }
    
  if(s==2)gSystem->Load("libDMRoot");
  if(s==3){
    //gROOT->ProcessLine(".x /home/vale/DMDS/dm2root_v7/src/macros/rootlogon.C"); 
    //gROOT->ProcessLine("gSystem->Load(\"libDMRoot\")");
    //gSystem->Load("libDMRoot");
    //gROOT->LoadMacro("myData_v5.C+");
    gROOT->ProcessLine(".L myData_v5.C++");
    //gROOT->ProcessLine("myrun()");
  }
  if(s==0)gROOT->ProcessLine(".q");
    if(s>3 || s<0) cout << "Non hai scelto nessuna opzione" << endl;
}while(s>3 || s<0);

}
