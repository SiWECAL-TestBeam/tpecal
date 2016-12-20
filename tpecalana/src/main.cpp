//
//  main.cpp
//  tpecalana
//
//  Created by Roman Poeschl on 28/7/15.
//  Copyright (c) 2015 CNRS. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <TFile.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <cmath>
#include <vector>
#include <map>
#include <TObject.h>
#include <TApplication.h>

#include "RunManager.h"
#include "ExperimentalSetup.h"
#include "AnaManager.h"
#include "ADCManager.h"
#include "global.h"
//#include "Analisys.h"

#include "boost/filesystem.hpp"   

using namespace::std;
using namespace globalvariables;
using namespace boost::filesystem; 



void ScanAnalysis(int step, int buffer, string datadirStr, string datadirStr_output ) {

  std::string path=datadirStr+"/0/";
  boost::filesystem::path apk_path(path);
  boost::filesystem::recursive_directory_iterator end;
      
  for (boost::filesystem::recursive_directory_iterator i(apk_path); i != end; ++i)   {
    const boost::filesystem::path cp = (*i);

    std::string mystring = cp.string();
    if(mystring.substr(mystring.length() - 4) == "root" ) {
      string scanvalue;
      if(globalvariables::getAnalysisType() == "scurves") {
	scanvalue =mystring.substr(mystring.find("trig")+4, 3);
	globalvariables::pushScanValue(atof(scanvalue.c_str()));
	std::cout<<atof(scanvalue.c_str())<<std::endl;
      }
    }
  }


  RunManager runmanager;    
  AnaManager anaManager;    anaManager.init();
  ADCManager adcManager;    adcManager.init();

 

  for (unsigned irun=0; irun < globalvariables::getScanVectorDoubles().size();irun++) {
    std::stringstream inputFileStr;
    //Set the ASU mappings for a given run
    std::vector<std::string> mapfilesStrvec;
    mapfilesStrvec.clear();
    mapfilesStrvec.push_back("/home/irles/2016/testbench_nov2016/mapping/tb-2015/fev10_chip_channel_x_y_mapping.txt");
    ExperimentalSetup::getInstance()->setRunSetup(mapfilesStrvec);

    std::cout<<"  ----------------------------------------- " <<std::endl;
    std::cout<<" New Trigger: "<< globalvariables::getScanVectorDoubles().at(irun) <<std::endl;

    TString scanstring;
    TString scanstring2;
    if(globalvariables::getAnalysisType() == "scurves") {
      scanstring="scurve_trig";
      scanstring2="trigger";
    }

    for(int ifile =0; ifile<64; ifile+=step) {
      std::cout<<" New set of measurements: file "<< ifile << " with trigger "<< globalvariables::getScanVectorDoubles().at(irun) <<" " << step << " " << ifile<< std::endl;
      inputFileStr.str("");
      inputFileStr << datadirStr << "/"<<ifile<<"/"+scanstring << globalvariables::getScanVectorDoubles().at(irun) << "_by_dif0.raw.root";
      runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
    }

    ExperimentalSetup::getInstance()->executeExperiment(runmanager.getDifFileVec(),10);

    TString output_path ;
    output_path =  TString(datadirStr_output) + "/"+ scanstring2 + TString::Format("%i",int(globalvariables::getScanVectorDoubles().at(irun) ) ) ;

 	
    if(globalvariables::getAnalysisType() == "scurves" ) 
      anaManager.acquireRunInformation(ExperimentalSetup::getInstance(), buffer);

    ////reset experimental setup and run manager
    ExperimentalSetup::getInstance()->reset();
    runmanager.reset();
	
  }

  if(globalvariables::getAnalysisType() == "scurves" )  {
    TString scurvefile= TString(datadirStr_output)+TString::Format("/Scurves_buff%i_",buffer);
    anaManager.displayResults( scurvefile,buffer );
  }

}



void NormalRun(string datadirStr, string datadirStr_output ) {


  RunManager runmanager;    
  AnaManager anaManager;    anaManager.init();
  ADCManager adcManager;    adcManager.init();

 
  std::stringstream inputFileStr;
  //Set the ASU mappings for a given run
  std::vector<std::string> mapfilesStrvec;
  mapfilesStrvec.clear();
  mapfilesStrvec.push_back("/home/irles/2016/testbench_nov2016/mapping/tb-2015/fev10_chip_channel_x_y_mapping.txt");
  ExperimentalSetup::getInstance()->setRunSetup(mapfilesStrvec);
  
  inputFileStr.str("");
  inputFileStr << datadirStr;
  runmanager.registerDifFile(new TFile(inputFileStr.str().c_str()));
  
  
  ExperimentalSetup::getInstance()->executeExperiment(runmanager.getDifFileVec(),10);
  
  TString output_path ;
  output_path =  TString(datadirStr_output) + "/" ;

  if(globalvariables::getAnalysisType() == "Pedestal" ) {
    adcManager.acquireRunInformation(ExperimentalSetup::getInstance(), output_path , true, false, "");
    adcManager.displayResults(output_path, true,false);
  }
  
  if(globalvariables::getAnalysisType() == "PedestalSignal" ) {
    adcManager.acquireRunInformation(ExperimentalSetup::getInstance(), output_path , true, true, "");
    adcManager.displayResults(output_path, true,true);
  }
 
  
  ////reset experimental setup and run manager
  ExperimentalSetup::getInstance()->reset();
  runmanager.reset();
  
}



int main(int argc, char **argv) 
{


  char** dummyappl;
  int dummyint = 0;
  TApplication fooApp("fooApp",&dummyint,dummyappl);

  if(argc < 4)
    {
      std::cout << " ----------------------------------------------- " << std::endl;
      std::cout << " *********************************************** " << std::endl;
      std::cout << " Technological Prototype Analysis Software " << std::endl;
      std::cout << "  " << std::endl;
      std::cout << " R. Poeschl & A. Irles " << std::endl;
      std::cout << " December 2016 " << std::endl;
      std::cout << "  " << std::endl;
      std::cout << " To run it: " << std::endl;
      std::cout << "  " << std::endl;
      std::cout << "./tpecalana for scan analysis (threshold scan, holdscan, etc) " << std::endl;
      std::cout << "    input_folder  == full path last subrun folder, containing converted root files. For No scan runs, this is the " << std::endl;
      std::cout << "    output_folder == relative path to the ouptut folder, which should be in input_folder/output_folder. " << std::endl;
      std::cout << "    analysis_type == scurves, holdscan" << std::endl;
      std::cout << "    enabled_chips == Number of enabled chips per dif (default 16) " << std::endl;
      std::cout << "    step          == in case of scans, the number of channels in each subrun (default 8) " << std::endl;
      std::cout << "    buffer        == buffer to analyze (for scans) 0-14. If 15, all are analyzed together. (default 0)" << std::endl;
      std::cout << "  " << std::endl;
      std::cout << "  " << std::endl;
      std::cout << "./tpecalana for normal runs" << std::endl;
      std::cout << "    input_file    == input root file " << std::endl;
      std::cout << "    output_folder == output folder (full path) " << std::endl;
      std::cout << "    analysis_type == Pedestal, PedestalSignal" << std::endl;
      std::cout << "    enabled_chips == Number of enabled chips per dif (default 16) " << std::endl;
      std::cout << "    step          == in case of scans, the number of channels in each subrun (default 8) " << std::endl;
      std::cout << "    buffer        == buffer to analyze (for scans) 0-14. If 15, all are analyzed together. (default 0)" << std::endl;
      return 1;
    }

  //Where are the data?
  std::string datadirStr="/home/irles/2016/testbench_nov2016/rawdata/daq_tests/20161215_123714/" + string(argv[1]);//
  std::string datadirStr_output="/home/irles/2016/testbench_nov2016/rawdata/daq_tests/20161215_123714/" + string(argv[2]);
  std::cout << "Directory where the data is: "<<datadirStr<<std::endl;

  if ( !exists( datadirStr ) ) {
    std::cout << " Data directory doesn't exist !!! STOP THE EXECUTABLE  "<<std::endl;
    return 0;
  }   
  if ( !exists( datadirStr_output ) ) {
    std::cout << " Data OUTPUT directory doesn't exist !!! STOP THE EXECUTABLE  "<<std::endl;
    std::cout << " Please, create the folder: "<<std::endl;
    std::cout << " mkdir "<<datadirStr_output<<std::endl;
    return 0;
  }

 
  globalvariables::setAnalysisType(TString(argv[3]));

  if(argc > 4) globalvariables::setEnabledChipsNumber(atoi(argv[4])); //
  else globalvariables::setEnabledChipsNumber(0); //

  int step = 8;
  if(argc > 5) step = atoi(argv[5]);
  int buffer = 0;
  if(argc > 6) buffer = atoi(argv[6]);


  if(globalvariables::getAnalysisType() == "scurves" || globalvariables::getAnalysisType() == "holdscan") {
    globalvariables::setGainAnalysis(1); //high =1, low =0
    globalvariables::setGlobal_deepAnalysis(false);
    ScanAnalysis(step, buffer, datadirStr, datadirStr_output) ;
  }

  if(globalvariables::getAnalysisType() == "Pedestal" || globalvariables::getAnalysisType() == "PedestalSignal") {
    globalvariables::setGainAnalysis(1); //high =1, low =0
    globalvariables::setGlobal_deepAnalysis(true);
    NormalRun(datadirStr, datadirStr_output) ;
  }


  // fooApp.Run();
  // return 1;

}
