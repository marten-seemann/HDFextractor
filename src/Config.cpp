//
//  Config.cpp
//  HDF
//
//  Created by Marten Seemann on 05.10.11.
//  Copyright (c) 2011 ---. All rights reserved.
//

#define DEBUGMESSAGES 0

#include "Config.h"

using namespace std;

Config::Config(const string& filename) {
  this->filename=filename;
  if(DEBUGMESSAGES) cout << "Constructor of Config called: " << filename << endl;
  if(!boost::filesystem::exists(filename)) {
    cerr << "File does not exist." << endl;
    exit(1);
  }
  file.open(filename.c_str(),ios::in);
  if(!file.is_open()) {
    cout << "Config file " << filename << " could not be opened.";
    exit(1);
  }
  readValues();
};

Config::~Config() {
  if(file.is_open()) file.close();
};

void Config::readValues() {
  if(DEBUGMESSAGES) cout << "readValues()" << endl;
  string line, key, value;
  while(file.good()) {
    getline(file,line);
    line=trim(line);
    if(line.length()==0) continue;
    if(line.substr(0,1)=="#") continue;
    istringstream line_stream(line);
    line_stream >> key >> value;
    values.insert(pair<string, string>(key,value));
  }
  
};


string Config::getValue(const string& name) const {
  value_map::const_iterator it=values.find(name);
  if(it==values.end()) {
    cerr << "No such config option supplied: " << name << endl;
    abort();
  }
  return it->second;
};

string Config::getFilename() const {
  return filename;
}