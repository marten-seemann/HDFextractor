#ifndef HDF_functions_h
#define HDF_functions_h

#include <iostream>
#include <sstream>
#include <vector>
#include <string>


typedef std::vector<long> layerdata_row;
typedef std::vector<layerdata_row> layerdata;

/// remove whitespace from the beginning of a string
/// @param str the string to be trimmed
/// @return the trimmed string
std::string& trim(std::string &str);
/// convert an int to a std::string
/// @param integer the int that should be converted
/// @result a string consisting of the value of the integer
std::string intToString(const int integer);

template <class T> std::string to_string (const T& t);

template <class T>
inline std::string to_string (const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}
#endif
