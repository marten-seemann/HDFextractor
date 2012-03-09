#ifndef HDF_HDFCryst_h
#define HDF_HDFCryst_h

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>


#include "Output.h"
#include "Ellipse.h"
#include "functions.h"
#include "H5Cpp.h"


/**
 * @brief A class for reading [HDF](http://www.hdfgroup.com) files generated by the rsv_mapper.py script
 *
 * This class is responsible for the reading of the HDF file. Methods are provided which allow to receive the stored data in various forms.
 * 
 */

class HDFCryst {
private:
  std::string filename;
  H5::H5File file;
  H5::Group root;
  int dims[3];
  int np;
  int intensity_complete; //-1 if not yet calculated
  /// the UBI matrix
  std::vector< std::vector<double> > ubi;
  //double ub[3][3];

public:  
  /// constructor
  /// @param filename the path to the HDF file
  explicit HDFCryst(std::string filename);
  /// destructor
  ~HDFCryst();
  /// get a whole layer from the hdf file
  /// @param dataset_name the name of the dataset, either "monitor" or "signal"
  /// @param orientation the surface normal to the layer to be read. Possible values: 0 = x => read the yz-plane, 1 = y => read the xz-plane, 2 = z => read the xy-plane
  /// @param number the coordinate specified by orientation
  /// @return the layer
  layerdata getLayer(const std::string dataset_name, const unsigned short orientation, const int number) const;
  layerdata_row getRow(const unsigned short orientation, const int coord1, const int coord2) const;
  layerdata_row getZylinderRow(const unsigned short orientation, const int coord1, const int coord2, const double site_a, const double site_b) const;
  void saveLayer(const std::string filename, const unsigned short orientation, const int number) const;
  std::vector<int> getDimensions() const;
  /// return the UBI matrix read from the header of the HDF file
  /** @return the UBI matrix as a two-dimensional vector (3x3)   */
  std::vector< std::vector<double> > getUbi() const;
  /// get the number of points used for creating this HDF file. This is the same value as the number of points specified for the rsv_mapper.py script.
  /// @return the number of points
  int getNP() const;
  
private:
  /// read the UBI matrix from the header of the HDF file
  void readUBI();
  /// @copybrief getNP()
  void readNP();
  /// read the dimensions of the reciprocal space encoded in the HDF file
  void readDimensions();
  
  
};

#endif
