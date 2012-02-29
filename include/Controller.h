//
//  Controller.h
//  HDF
//
//  Created by Marten Seemann on 10.10.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

#ifndef HDF_Controller_h
#define HDF_Controller_h

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <ctime>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


#include "gnuplot_i/gnuplot_i.h"

#include "Config.h"
#include "Output.h"
#include "HDFCryst.h"
#include "functions.h"




class Controller {
private:
  Config* config;
  HDFCryst* hdf;
  int dir_index;
  int run;
  std::string direction;
  std::string output_dirname;
  std::string streak_dirname; // output directory for the streak files
  std::vector<int> dimensions;
  
public:
  explicit Controller(int argc, const char * argv[]);
  ~Controller();
  
private:
  void readConfigFile(const std::string filename);
  void showInfos() const;
  void openHDF(const std::string filename);
  void createOutputDirs();
  void getDirection();
  void doRun1();
  void doRun2();
  void plotSlice(const std::string datafile, const int number) const;
  void plotRow(const std::string datafile) const;
  std::string getLogFileName(const std::string prefix, const std::string ending) const;
  
};


#endif
