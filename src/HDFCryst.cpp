/// print debug messages
#define DEBUGMESSAGES 0

#include "HDFCryst.h"


using namespace std;
#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

HDFCryst::HDFCryst(string filename) {
  intensity_complete = -1;
  this->filename = filename;
  try {
    Exception::dontPrint();
    file = H5File(filename, H5F_ACC_RDONLY);
  }
  catch (FileIException error) {
    cerr << "File " << filename << " not found." << endl;
    exit(1);
  }
  root = file.openGroup("/");
  readHDFHeader();
}

HDFCryst::~HDFCryst() {
  if(DEBUGMESSAGES) cout << "Destructor of HDFCryst called." << endl;
  root.close();
  file.close();
}

void HDFCryst::readHDFHeader() {
  if(DEBUGMESSAGES) cout << "File opened: " << file.getFileName() << endl;
  readDimensions();
  if(DEBUGMESSAGES) cout << "Reading NP." << endl;
  readNP();
  if(DEBUGMESSAGES) cout << "Reading UBI." << endl;
  readUBI();
}

void HDFCryst::readUBI() {
  Attribute att = root.openAttribute("ubi");
  DataSpace space = att.getSpace();
  int rank = space.getSimpleExtentNdims();
  // rank must be 2, since ubi is a matrix
  if(rank != 2) abort();
  double ubiarray[3][3];
  att.read(PredType::IEEE_F64LE, ubiarray);
  
  //write ubiarray to std::vector ubi
  for(int i=0; i<3; i++) {
    vector<double> vec;
    for(int j=0; j<3; j++) {
      vec.push_back(ubiarray[i][j]);
    }
    ubi.push_back(vec);
  }

  space.close();
  att.close();
};

//read the np value from the HDF file
void HDFCryst::readNP() {
  Attribute att = root.openAttribute("np");
  DataSpace space = att.getSpace();
  int rank = space.getSimpleExtentNdims();
  // rank must be 0, since np is a scalar value
  if(rank != 0) exit(1);
  int value[1];
  att.read(PredType::STD_I32LE, value);
  np = value[0];
  space.close();
  att.close();
};

void HDFCryst::readDimensions() {
  Attribute att = root.openAttribute("bounds");
  DataSpace space = att.getSpace();
  double values[6];
  att.read(PredType::IEEE_F64LE,values);
  for(int i=0; i<3; i++) {
    dims[i] = values[i*2+1] - values[i*2] + 1;
  }
   
  space.close();
  att.close();
};

vector<int> HDFCryst::getDimensions() const {
  vector<int> values(3);
  values[0] = dims[0];
  values[1] = dims[1];
  values[2] = dims[2];
  return values;
};

vector <vector<double> > HDFCryst::getUbi() const {
  return ubi;
};

int HDFCryst::getNP() const {
  return np;
};


/// @todo: return reference or pointer... gives 5% performance increase!
layerdata HDFCryst::getLayer(const string dataset_name, const unsigned short orientation, const int number) const {
  if(orientation>2) {
    cerr << "Error: Orientation must be a value between 0 and 2." << endl;
    abort();  
  }
  DataSet dataset = file.openDataSet( dataset_name);
  DataSpace filespace = dataset.getSpace();
  
  hsize_t rdims[3] = {1,1,1};
  rdims[(orientation+1)%3] = dims[(orientation+1)%3];
  rdims[(orientation+2)%3] = dims[(orientation+2)%3];

  const unsigned long prod = rdims[0]*rdims[1]*rdims[2];
  DataSpace mspace(3, rdims);
  
  layerdata values(dims[(orientation+2)%3], layerdata_row(dims[(orientation+1)%3]));
  
  hsize_t offset[3] = { 0, 0, 0 };
  offset[orientation] = number;
  hsize_t  count[3] = { rdims[0], rdims[1], rdims[2] }; //4,1
  //float column[prod];  // buffer for column to be read
  float* column = new float[prod];
  filespace.selectHyperslab(H5S_SELECT_SET, count, offset);
  dataset.read(column, PredType::NATIVE_FLOAT, mspace, filespace );
    
  unsigned long counter=0;
  for(int i=0; i<dims[(orientation+1)%3]; i++) {
    for(int j=0; j<dims[(orientation+2)%3]; j++) {
      values[j][i] = column[counter];
      counter++;
    }
  }
  delete[] column;
  return values;
};


vector<layerdata_row> HDFCryst::getRow(const unsigned short orientation, const int coord1, const int coord2) const {
  vector<layerdata_row> res;
  cout << "Extracting from dataset: signal" << endl;
  res.push_back( getRowData("signal", orientation, coord1, coord2) );
  cout << "Extracting from dataset: monitor" << endl;
  res.push_back( getRowData("monitor", orientation, coord1, coord2) );
  return res;
}


layerdata_row HDFCryst::getRowData(const string dataset_name, const unsigned short orientation, const int coord1, const int coord2) const {
  layerdata_row values;
  DataSet dataset = file.openDataSet( dataset_name );
  DataSpace filespace = dataset.getSpace();
  
  float* column = new float[dims[orientation]];
  hsize_t rdims[3] = {1,1,1};
  rdims[orientation] = dims[orientation];
  DataSpace mspace(3, rdims);

  hsize_t offset[3] = { 0, 0, 0 };
  offset[(orientation+1)%3] = coord1;
  offset[(orientation+2)%3] = coord2;
  hsize_t count[3] = { rdims[0], rdims[1], rdims[2] };
  
  filespace.selectHyperslab(H5S_SELECT_SET, count, offset);
  dataset.read(column, PredType::NATIVE_FLOAT, mspace, filespace );
  
  for(int i=0; i<dims[orientation]; i++) {
    values.push_back(column[i]);
  }

  dataset.close();
  filespace.close();
  
  return values;
};


vector<layerdata_row> HDFCryst::getCylinderRow(const unsigned short orientation, const int coord1, const int coord2, const double site_a, const double site_b) const {
  vector<layerdata_row> res;
  cout << "Extracting from dataset: signal" << endl;
  res.push_back( getCylinderRowData("signal", orientation, coord1, coord2, site_a, site_b) );
  cout << "Extracting from dataset: monitor" << endl;
  res.push_back( getCylinderRowData("monitor", orientation, coord1, coord2, site_a, site_b) );
  return res;
}

layerdata_row HDFCryst::getCylinderRowData(const string dataset_name, const unsigned short orientation, const int coord1, const int coord2, const double site_a, const double site_b) const {
  layerdata_row result;
  Ellipse circ(0, 0, site_a, site_b);
  //  cout << "Ellipse with " << site_a << " x " << site_b << endl;

  int min_j = floor(-site_a - 1);
  int max_j = ceil(site_a + 1);
  int min_k = floor(-site_b - 1);
  int max_k = ceil(site_b + 1);

  // get fractions from the ellipse
  vector<double> fields;
  for(int j=min_j; j<max_j; j++) {
    for(int k=min_k; k<max_k; k++) {
      fields.push_back(circ.getArea(j-0.5, 1, k-0.5, 1));
    }
  }
  
  // read the data from hdf
  vector<layerdata_row> columns;
  for(int j=min_j; j<max_j; j++) {
    for(int k=min_k; k<max_k; k++) {
      columns.push_back(getRowData(dataset_name, orientation, coord1+j, coord2+k));
    }
  }
  
  for(int i=0; i<dims[orientation]; i++) {
    long double sum = 0;
    for(int j=0; j<columns.size(); j++) {
      sum += fields.at(j)*columns.at(j).at(i);
    }
    result.push_back(sum);
  }
  return result;
};
