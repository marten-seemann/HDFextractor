#ifndef HDF_Config_h
#define HDF_Config_h

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
#include <cstring>
#include <boost/filesystem.hpp>

#include "functions.h"

typedef std::map<std::string,std::string> value_map;

/// A class for reading the config file
/*!
reads the config file and provides a function to access the configuration options 
*/

class Config {
private:
  /// the path to the configuration file
  std::string filename;
  std::ifstream file;
  /// here the configuration values read from the configuration will are stored
  value_map values;
  
private:
  /// open the config file and read the values, store them to the values variable
  void readValues();
  
public:
  /// constructor
  /// @param filename the path to the config file to be read
  explicit Config(const std::string& filename);
  /// destructor
  ~Config();
  /// get a value. Aborts the execution if this value is not supplied in the configuration file
  /// @param name the name of the configuration option
  /// @return the value of the configuration option, if exists
  std::string getValue(const std::string& name) const;
  /// get the filename of the configuration file
  /// @return the filename, as given to the constructor
  std::string getFilename() const;
};

#endif