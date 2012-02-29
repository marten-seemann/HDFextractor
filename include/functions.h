//
//  functions.h
//  HDF
//
//  Created by Marten Seemann on 08.09.11.
//  Copyright 2011 ---. All rights reserved.
//

#ifndef HDF_functions_h
#define HDF_functions_h

#include <iostream>
#include <sstream>
#include <vector>
#include <string>


typedef std::vector<long> layerdata_row;
typedef std::vector<layerdata_row> layerdata;

std::string& trim(std::string &str);
std::string intToString(const int a);

template <class T> std::string to_string (const T& t);

template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}
#endif
