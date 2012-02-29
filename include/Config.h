//
//  Config.h
//  HDF
//
//  Created by Marten Seemann on 05.10.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

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

class Config {
private:
  std::string filename;
  std::ifstream file;
  value_map values;
  
private:
  void readValues();
  
public:
  explicit Config(const std::string& filename);
  ~Config();
  std::string getValue(const std::string& name) const;
  std::string getFilename() const;

};



#endif
