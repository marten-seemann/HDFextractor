#ifndef HDF_Output_h
#define HDF_Output_h

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

/**
 * @brief A class for writing things to an output file, or to standard output
 *
 * This class writes to *one* output file, or to standard output
 * 
 */



class Output {
  /// true if output is directed to an output file, false if it goes to standard output
  bool filewrite; 
  /// the path to the output file
  /** will only be set if filewrite is true */
  std::ofstream file;
  
public:
  /// @brief default constructor
  /** all output will be directed to standard output */
  Output();
  /// constructor
  /** if this constructor is called, the output will be directed to an output file
      @param filename path to the output file. The file will be created if it does not yet exist. */
  Output(std::string filename);
  /// destructor
  /** closes the file if necessary */
  ~Output();
  template <typename T>  void write(const T& value);
  template <typename T>  inline void operator<<(const T& value);
};



template <typename T> 
void Output::write(const T& value) {
  if(filewrite) {
    file << value;
  }
  else std::cout << value << std::endl;
};


template <typename T> 
void Output::operator<<(const T& value) {
  write(value);
};

#endif
