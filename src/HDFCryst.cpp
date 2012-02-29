//
//  HDFCryst.cpp
//  HDF
//
//  Created by Marten Seemann on 08.09.11.
//  Copyright 2011 ---. All rights reserved.
//

#define DEBUGMESSAGES 0

#include "HDFCryst.h"


using namespace std;
#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

HDFCryst::HDFCryst(string filename) {
  intensity_complete=-1;
  this->filename=filename;
  try {
    Exception::dontPrint();
    file = H5File( filename, H5F_ACC_RDONLY );
  }
  catch (FileIException error) {
    cerr << "File " << filename << " not found." << endl;
    exit(1); //abort();
  }
  root = file.openGroup("/");

  if(DEBUGMESSAGES) cout << "File opened: " << file.getFileName() << endl;
  readDimensions();
  if(DEBUGMESSAGES) cout << "Reading NP." << endl;
  readNP();
  if(DEBUGMESSAGES) cout << "Reading UBI." << endl;
  readUBI();

}

HDFCryst::~HDFCryst() {
  if(DEBUGMESSAGES) cout << "Destructor of HDFCryst called." << endl;
  root.close();
  file.close();
}



//read the UBI-Matrix from the HDF file
void HDFCryst::readUBI() {
  Attribute att = root.openAttribute("ubi");
  DataSpace space = att.getSpace();
  int rank = space.getSimpleExtentNdims();
  // rank must be 2, since ubi is a matrix
  if(rank!=2) abort();
  double ubiarray[3][3];
  att.read(PredType::IEEE_F64LE,ubiarray);
  
  //cout << "{{ " << ubi[0][0] << ", " << ubi[0][1] << ", " << ubi[0][2] << " }, " << endl;
  //cout << "{ " << ubi[1][0] << ", " << ubi[1][1] << ", " << ubi[1][2] << " }, " <<endl;
  //cout << "{ " << ubi[2][0] << ", " << ubi[2][1] << ", " << ubi[2][2] << " }}" << endl;
  
  //write ubiarray to std::vector ubi
  for(int i=0;i<3;i++) {
    vector<double> vec;
    for(int j=0;j<3;j++) {
      vec.push_back(ubiarray[i][j]);
    }
    ubi.push_back(vec);
  }

  space.close();
  att.close();
  
  // evaluate ubi
  // vector lengthes
  /*
  double len1 = sqrt(ubi[0][0]*ubi[0][0] + ubi[0][1]*ubi[0][1] + ubi[0][2]*ubi[0][2]);
  double len2 = sqrt(ubi[1][0]*ubi[1][0] + ubi[1][1]*ubi[1][1] + ubi[1][2]*ubi[1][2]);
  double len3 = sqrt(ubi[2][0]*ubi[2][0] + ubi[2][1]*ubi[2][1] + ubi[2][2]*ubi[2][2]);

  cout << "Vec 1: " << len1 << endl;
  cout << "Vec 2: " << len2 << endl;
  cout << "Vec 3: " << len3 << endl;
  
  double angle12 = acos((ubi[0][0]*ubi[1][0] + ubi[0][1]*ubi[1][1] + ubi[0][2]*ubi[1][2])/(len1*len2))*M_1_PI*180;
  double angle13 = acos((ubi[0][0]*ubi[2][0] + ubi[0][1]*ubi[2][1] + ubi[0][2]*ubi[2][2])/(len1*len3))*M_1_PI*180;
  double angle23= acos((ubi[1][0]*ubi[2][0] + ubi[1][1]*ubi[2][1] + ubi[1][2]*ubi[2][2])/(len2*len3))*M_1_PI*180;
  cout << "Angle between Vec 1 and Vec 2: " << angle12 << endl;
  cout << "Angle between Vec 1 and Vec 3: " << angle13 << endl;
  cout << "Angle between Vec 2 and Vec 3: " << angle23 << endl;
   */
};

//read the np value from the HDF file
void HDFCryst::readNP() {
  Attribute att = root.openAttribute("np");
  DataSpace space = att.getSpace();
  int rank = space.getSimpleExtentNdims();
  // rank must be 0, since np is a scalar value
  if(rank!=0) exit(1);
  int value[1];
  att.read(PredType::STD_I32LE,value);
  np=value[0];
  space.close();
  att.close();
};

void HDFCryst::readDimensions() {
  Attribute att = root.openAttribute("bounds");
  DataSpace space = att.getSpace();
  double values[6];
  att.read(PredType::IEEE_F64LE,values);
  for(int i=0;i<3;i++) {
    dims[i]=values[i*2+1]-values[i*2]+1;
  }
  
  /*
  cout << "Dimensions: ";
  for(int i=0; i<3; i++) {
    if(i>0) cout << " x ";
    cout << dims[i];
  }
  cout << endl;
  */
   
  space.close();
  att.close();
};

vector<int> HDFCryst::getDimensions() const {
  vector<int> values(3);
  values[0]=dims[0];
  values[1]=dims[1];
  values[2]=dims[2];
  return values;
};

vector <vector<double> > HDFCryst::getUbi() const {
  return ubi;
};

int HDFCryst::getNP() const {
  return np;
};

void HDFCryst::calcUB() {
  
};


void HDFCryst::saveLayer(const string filename, const unsigned short orientation, const int number) const {
  layerdata signal=getLayer2("signal",orientation, number);
  layerdata monitor=getLayer2("monitor", orientation, number);
  
  Output file(filename);
  file << "#x y signal monitor\n";
  for(int i=0;i<signal.size();i++) {
    layerdata_row signal2=signal.at(i);
    layerdata_row monitor2=monitor.at(i);
    for(int j=0;j<signal2.size();j++) {
      file << j;
      file << " ";
      file << i;
      file << " ";
      file << signal2.at(j);
      file << " ";
      file << monitor2.at(j);
      file << "\n";
    }
    file << "\n";
  }
};

void HDFCryst::getCompleteIntensity() {
  if(intensity_complete>0) cout << endl << "WARNING: Complete Intensity already calculated!" << endl << endl;
  for(int i=0;i<dims[0];i++) {
    layerdata temp=getLayer("monitor", 0, i);
    // Todo: summieren ueber temp
  }
};

// CCCCCCCCCCAAAAAAAAUUUUUUUUTTTTTTTTTTIIIIIIIIIOOOOOOOOOONNNNNNNNNNNN: deprecated
layerdata HDFCryst::getLayer(const string dataset_name, const unsigned short orientation, const int number) const {
  cerr << "USING DEPRECATED FUNCTION getLayer(). USE getLayer2() INSTEAD FOR IMPROVED PERFORMANCE." << endl;
  if(orientation>2) {
    cerr << "Error: Orientation must be a value between 0 and 2." << endl;
    abort();  
  }
  DataSet dataset = file.openDataSet( dataset_name);
  DataSpace filespace = dataset.getSpace();
  
  hsize_t rdims[3]={1,1,1};
  rdims[(orientation+2)%3]=dims[(orientation+2)%3];
  unsigned long prod=rdims[0]*rdims[1]*rdims[2];
  DataSpace mspace(3, rdims);
      
  layerdata values(prod, layerdata_row(dims[(orientation+1)%3]));
  
  for(int col=0;col<dims[(orientation+1)%3];col++) {
    hsize_t offset[3] = { 0, 0, 0 };
    offset[orientation]=number;
    offset[(orientation+1)%3]=col;
    hsize_t  count[3] = { rdims[0], rdims[1], rdims[2] }; //4,1
    float column[prod];  // buffer for column to be read
    filespace.selectHyperslab( H5S_SELECT_SET, count, offset);
    dataset.read( column, PredType::NATIVE_FLOAT, mspace, filespace );
    
    for(int i=0;i<prod;i++) {
      values[i][col] = column[i];
    }
  }
  // cout << prod << " x " << dims[(orientation+1)%3] << endl;
  return values;
};

//ToDo: return reference or pointer... gives 5% performance increase!
layerdata HDFCryst::getLayer2(const string dataset_name,const unsigned short orientation, const int number) const {
  if(orientation>2) {
    cerr << "Error: Orientation must be a value between 0 and 2." << endl;
    abort();  
  }
  DataSet dataset = file.openDataSet( dataset_name);
  DataSpace filespace = dataset.getSpace();
  
  hsize_t rdims[3]={1,1,1};
  rdims[(orientation+1)%3]=dims[(orientation+1)%3];
  rdims[(orientation+2)%3]=dims[(orientation+2)%3];

  unsigned long prod=rdims[0]*rdims[1]*rdims[2];
  DataSpace mspace(3, rdims);
  
  layerdata values(dims[(orientation+2)%3], layerdata_row(dims[(orientation+1)%3]));
  
//  for(int col=0;col<dims[(orientation+1)%3];col++) {
  hsize_t offset[3] = { 0, 0, 0 };
  offset[orientation]=number;
  //offset[(orientation+1)%3]=col;
  hsize_t  count[3] = { rdims[0], rdims[1], rdims[2] }; //4,1
  float column[prod];  // buffer for column to be read
  filespace.selectHyperslab( H5S_SELECT_SET, count, offset);
  dataset.read( column, PredType::NATIVE_FLOAT, mspace, filespace );
    
  unsigned long counter=0;
  for(int i=0;i<dims[(orientation+1)%3];i++) {
    for(int j=0;j<dims[(orientation+2)%3];j++) {
      values[j][i] = column[counter];
      counter++;
    }
  }
  // cout << prod << " x " << dims[(orientation+1)%3] << endl;
  return values;
};


layerdata_row HDFCryst::getRow(const unsigned short orientation, const int coord1, const int coord2) const {
  layerdata_row values;
  DataSet dataset = file.openDataSet( "signal" );
  DataSpace filespace = dataset.getSpace();
  
  float column[dims[orientation]];
  hsize_t rdims[3]={1,1,1};
  rdims[orientation]=dims[orientation];
  DataSpace mspace(3, rdims);

  hsize_t offset[3]={ 0, 0, 0 };
  offset[(orientation+1)%3]=coord1;
  offset[(orientation+2)%3]=coord2;
  hsize_t  count[3] = { rdims[0], rdims[1], rdims[2] }; //4,1
  
  filespace.selectHyperslab( H5S_SELECT_SET, count, offset);
  dataset.read( column, PredType::NATIVE_FLOAT, mspace, filespace );
  
  //cout << "Dim: " << dims[orientation] << endl;
  //values.resize(dims[orientation]);
  for(int i=0;i<dims[orientation];i++) {
    //cout << i << " " << column[i] << endl;
    values.push_back(column[i]);
  }

  dataset.close();
  filespace.close();
  
  return values;
};

layerdata_row HDFCryst::getZylinderRow(const unsigned short orientation, const int coord1, const int coord2, const double site_a, const double site_b) const {
  layerdata_row result;
  //ToDo: use radius for determining the surface of the "zylinder"
  //double site_a=radius;
  //double site_b=radius;
  Ellipse circ(0,0,site_a,site_b);
  cout << "Ellipse with " << site_a << " x " << site_b << endl;

  int min_j=floor(-site_a-1);
  int max_j=ceil(site_a+1);
  int min_k=floor(-site_b-1);
  int max_k=ceil(site_b+1);

  // get fractions from the ellipse
  vector<double> fields;
  for(int j=min_j;j<max_j;j++) {
    for(int k=min_k;k<max_k;k++) {
      fields.push_back(circ.getArea(j-0.5,1,k-0.5,1));
      //cout << "(" << j-0.5 << ", " << k-0.5 << "): " << circ.getArea(j-0.5,1,k-0.5,1) << endl;
    }
  }
  
  // read the data from hdf
  vector<layerdata_row> columns;
  for(int j=min_j;j<max_j;j++) {
    for(int k=min_k;k<max_k;k++) {
      columns.push_back(getRow(orientation,coord1+j,coord2+k));
    }
  }
  
  /*
  for(int j=min_j;j<max_j;j++) {
    for(int k=min_k;k<max_k;k++) {
      cout << "j: " << j << ", \tk: " << k << "\t area: " << circ.getArea(j-0.5,1,k-0.5,1) << endl;
    }
  }
   */
  
  for(int i=0;i<dims[orientation];i++) {
    unsigned long sum=0;
    for(int j=0;j<columns.size();j++) {
      sum+=fields.at(j)*columns.at(j).at(i);
    }
    result.push_back(sum);
    //cout << i << "\t" << sum << endl;
  }
  return result;
};
