//
//  Output.cpp
//  HDF
//
//  Created by Marten Seemann on 04.10.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

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