#include "Output.h"

using namespace std;

Output::Output() {
  filewrite=false;
};

Output::Output(string filename) {
  filewrite=true;
  file.open(filename.c_str(),ios::trunc);
  if(!file.is_open()) {
    cout << "Unable to open output file " << filename << "." << endl;
  }
};

Output::~Output() {
  if(filewrite && file.is_open()) file.close();
};