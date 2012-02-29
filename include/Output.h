//
//  Output.h
//  HDF
//
//  Created by Marten Seemann on 04.10.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

#ifndef HDF_Output_h
#define HDF_Output_h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
//#include "functions.h"

class Output {
  bool filewrite;
  std::ofstream file;
  
public:
  Output();
  Output(std::string filename);
  ~Output();
  template <typename T>  void write(const T& value);
  template <typename T>  inline void operator<<(const T& value);
};



template <typename T> void Output::write(const T& value) {
  if(filewrite) {
    file << value;
  }
  else std::cout << value << std::endl;
};


template <typename T> void Output::operator<<(const T& value) {
  write(value);
};

#endif
