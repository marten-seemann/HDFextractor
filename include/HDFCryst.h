//
//  HDFCryst.h
//  HDF
//
//  Created by Marten Seemann on 08.09.11.
//  Copyright 2011 ---. All rights reserved.
//

#ifndef HDF_HDFCryst_h
#define HDF_HDFCryst_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

//#include <gsl/gsl_eigen.h>
//#include <gsl/gsl_math.h> 
//#include <gsl/gsl_blas.h>

#include "Output.h"
#include "Circle.h"
#include "functions.h"
#include "H5Cpp.h"


class HDFCryst {
private:
  std::string filename;
  H5::H5File file;
  H5::Group root;
  int dims[3];
  int np;
  int intensity_complete; //-1 if not yet calculated
  std::vector< std::vector<double> > ubi;
  //double ub[3][3];

public:
  explicit HDFCryst(std::string filename);
  ~HDFCryst();
  layerdata getLayer(const std::string dataset_name, const unsigned short orientation, const int number) const;
  layerdata getLayer2(const std::string dataset_name, const unsigned short orientation, const int number) const;
  layerdata_row getRow(const unsigned short orientation, const int coord1, const int coord2) const;
  layerdata_row getZylinderRow(const unsigned short orientation, const int coord1, const int coord2, const double site_a, const double site_b) const;
  void saveLayer(const std::string filename, const unsigned short orientation, const int number) const;
  void getCompleteIntensity();
  std::vector<int> getDimensions() const;
  std::vector< std::vector<double> > getUbi() const;
  int getNP() const;
  
private:
  void readUBI();
  void calcUB();
  void readNP();
  void readDimensions();
  
  
};

#endif
