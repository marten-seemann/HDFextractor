#include "Controller.h"

using namespace std;
using namespace boost::filesystem;
namespace po = boost::program_options;

Controller::Controller(int argc, const char * argv[]) {
  po::options_description cmdline_options;
  po::options_description generic("Generic options");
  generic.add_options()
    ("help", "produce this help message")
    ("version", "show version information")
  ;
  
  po::options_description info("Obtain general information from HDF file");
  info.add_options()
    ("show-info", "show general information about the HDF file")
    ("hdf", po::value<string>(), "the HDF file")
  ;
  
  po::options_description extract("Extract information from HDF file");
  extract.add_options()
    ("config", po::value<string>(), "the configuration file")
  ;
  cmdline_options.add(generic);
  cmdline_options.add(info);
  cmdline_options.add(extract);
  
  po::variables_map vm;
  po::positional_options_description p;
  
  po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
  po::notify(vm);  
  
  // non-valid command line options (or help) => show usage
  if (vm.size() == 0 || vm.count("help") || (vm.count("show-info") && !vm.count("hdf"))) {
    cout << cmdline_options << "\n";
    exit(1);
  }
  
  if(vm.count("version")) {
    cout << "HDFextractor, version 1.0.1" << endl;
    exit(1);
  }
    
  if(vm.count("show-info")) {
    openHDF(vm["hdf"].as<string>());
    printInfos();
  }
  if(vm.count("config")) {
    string configfile = vm["config"].as<string>();
    cout << "Reading configuration from: " << configfile << endl;
    readConfigFile(configfile);
  }
  exit(1); // needed to prevent a dubious "Misaligned address error"
};


Controller::~Controller() {
  if(config) delete config;
  if(hdf) delete hdf;
};


//read data from a HDF file
void Controller::readConfigFile(const string filename) {
  config = new Config(filename);
  cout << "Run " << config->getValue("run") << endl;
  openHDF(config->getValue("file"));
  dimensions = hdf->getDimensions();

  run=atoi(config->getValue("run").c_str());
  getDirection();
  createOutputDirs();

  // Run 1
  if(run==1) doRun1();
  else if(run==2) doRun2();
  else {
    exit(1);
  }
};


// show infos about the HDF file, like dimensions, UBI and NP value
void Controller::printInfos() const {
  // print Dimensions
  vector<int> dimensions = hdf->getDimensions();
  cout << endl;
  cout << "Dimensions:" << endl;
  cout << "\tx\ty\tz" << endl;
  cout << "\t" << dimensions.at(0) << "\t" << dimensions.at(1) << "\t" << dimensions.at(2) << endl;

  //print UBI
  cout << endl;
  cout << "UBI Matrix: " << endl;
  vector< vector<double> > ubi=hdf->getUbi();
  for(int i=0; i<3; i++) {
    cout << "\t";
    for(int j=0; j<3; j++) {
      cout << ubi.at(i).at(j);
      if(ubi.at(i).at(j) < 0) cout << "\t";
      else cout << "\t\t";
    }
    cout << endl;
  }
  cout << endl;
  
  cout << "NP (Resolution):\t" << hdf->getNP() << endl << endl;
  
  // evaluate ubi
  // vector lengthes
  cout << "UBI dimensions: " << endl;
  double len1 = sqrt(ubi[0][0]*ubi[0][0] + ubi[0][1]*ubi[0][1] + ubi[0][2]*ubi[0][2]);
  double len2 = sqrt(ubi[1][0]*ubi[1][0] + ubi[1][1]*ubi[1][1] + ubi[1][2]*ubi[1][2]);
  double len3 = sqrt(ubi[2][0]*ubi[2][0] + ubi[2][1]*ubi[2][1] + ubi[2][2]*ubi[2][2]);

  cout << "Vector 1: " << len1 << endl;
  cout << "Vector 2: " << len2 << endl;
  cout << "Vector 3: " << len3 << endl;

  // angles between the vectors
  double angle12 = acos((ubi[0][0]*ubi[1][0] + ubi[0][1]*ubi[1][1] + ubi[0][2]*ubi[1][2])/(len1*len2))*M_1_PI*180;
  double angle13 = acos((ubi[0][0]*ubi[2][0] + ubi[0][1]*ubi[2][1] + ubi[0][2]*ubi[2][2])/(len1*len3))*M_1_PI*180;
  double angle23= acos((ubi[1][0]*ubi[2][0] + ubi[1][1]*ubi[2][1] + ubi[1][2]*ubi[2][2])/(len2*len3))*M_1_PI*180;

  // \370 is the degree sign. Thx to Windows we need to write it like this
  string degreesign;
#ifdef _WIN32
  degreesign = "\370";
#else
  degreesign = "°";
#endif
  cout << "Angle between Vector 1 and Vector 2: " << angle12 << degreesign << endl;
  cout << "Angle between Vector 1 and Vector 3: " << angle13 << degreesign << endl;
  cout << "Angle between Vector 2 and Vector 3: " << angle23 << degreesign << endl;
}

void Controller::openHDF(const string filename) {
  hdf = new HDFCryst(filename);
  cout << "HDF file opened: " << filename << endl;
}


// check if the output directory exists and create neccessary folder structure
void Controller::createOutputDirs() {
  output_dirname = config->getValue("output");
  if(exists(output_dirname) && !is_directory(output_dirname)) {
    cerr << "Output file " << output_dirname << " exists, but is not a directory." << endl;
    exit(1);
  }
  else if(!exists(output_dirname)) {
    if(!create_directory(output_dirname)) {
      cerr << "Could not create output directory " << output_dirname << endl;
      exit(1);
    }
  }
  else cout << "Using existing directory " << output_dirname << " for output." << endl;

  // create directory for the slices
  if(run==1) {
    //if(exists(output_dirname+"/"+direction)) remove_all(output_dirname+"/"+direction);
    if(!exists(output_dirname + "/" + direction) && (!create_directory(output_dirname + "/" + direction) || direction.length() == 0)) {
      cerr << "Could not create output directory " << output_dirname + "/" + direction << endl;
      exit(1);
    }
    cout << "Saving into directory: " << output_dirname + "/" + direction << endl << endl;
  }
  else { // run=2: create directory for the streaks
    if(!exists(output_dirname+"/streaks")) {
      if(!create_directory(output_dirname+"/streaks")) {
        cerr << "Could not create output directory " << output_dirname + "/streaks" << endl;
        exit(1);
      }
    }
    streak_dirname=output_dirname + "/streaks/" + config->getValue("streak_directory");
    if(exists(streak_dirname)) {
      cerr << "Streak output directory " << streak_dirname << " already exists. Please choose another one." << endl;
      exit(1);
    }
    else {
      if(!create_directory(streak_dirname)) {
        cerr << "Could not create output directory " << streak_dirname << endl;
        exit(1);
      }
    }
    
  }

};

// evaluate the direction input option
void Controller::getDirection() {
  direction = config->getValue("direction");
  if(direction == "x") dir_index = 0;
  else if(direction == "y") dir_index = 1;
  else if(direction == "z") dir_index = 2;
  else {
    cerr << "Direction must be either x, y oder z." << endl;
    exit(1);
  }
};


void Controller::doRun1() {
  string output_dir = output_dirname + "/" + direction;
  // copy the config file
  boost::filesystem::copy_file(config->getFilename(), output_dir+"/"+getLogFileName("config", "log"));
  // for(int i=0;i<dimensions.at(dir_index);i++) {
  int start_slice, end_slice;
  if(config->getValue("slices_start") == "center") {
    int num = atoi(config->getValue("slices_number").c_str());
    start_slice = floor(static_cast<double>(dimensions.at(dir_index)/2-num/2));
    end_slice = ceil(static_cast<double>(dimensions.at(dir_index)/2+num/2));
  }
  else if(config->getValue("slices_start") == "all") {
    start_slice = 1;
    end_slice = static_cast<double>(dimensions.at(dir_index));
  }
  else {
    start_slice = max(1, atoi(config->getValue("slices_start").c_str()));
    end_slice = min(start_slice+atoi(config->getValue("slices_number").c_str()), dimensions[dir_index]);
  }
  if(start_slice >= end_slice) {
    cerr << "Parameter slices_start must be smaller than the dimension of the HDF file." << endl;
    exit(1);
  }
  
  cout << "Calculating slices." << endl;

  
  bool plot;
  // disable gnuplotting on windows, regardless of what the user specified in the config file
#ifdef _WIN32
 plot = false;
#else
  if(config->getValue("plot") != "0") plot=true;
  else plot=false;
#endif
  
  for(int i=start_slice; i<end_slice; i++) {
    cout << "Saving slice " << (i+1) << " / " << dimensions.at(dir_index) << endl;
    string filename = output_dir+"/slice" + to_string(i) + ".dat";
    hdf->saveLayer(filename, dir_index, i);
    if(plot) {
	  cout << "Plotting slice " << (i+1) << "..." << endl;
      plotSlice(filename, i);
    }
  }
};

void Controller::doRun2() {
  // copy the config file  
  boost::filesystem::copy_file(config->getFilename(), streak_dirname + "/" + getLogFileName("config", "log"));

  vector<layerdata_row> row;
  
  int x, y, z;
  if(direction != "x") x = atoi(config->getValue("offset_x").c_str());
  if(direction != "y") y = atoi(config->getValue("offset_y").c_str());
  if(direction != "z") z = atoi(config->getValue("offset_z").c_str());

  if(config->getValue("mode") == "cylinder") {
    double size_x, size_y, size_z;
    if(direction != "x") {
      size_x = atof(config->getValue("size_x").c_str());
      if(x+size_x > dimensions[0]-2 || x-size_x <= 2) {
        cerr << "The cylinder must completely lie inside the dimensions of the HDF file. Please check the parameters size_x and offset_x." << endl;
        exit(1);
      }
    }
    if(direction != "y") {
      size_y = atof(config->getValue("size_y").c_str());
      if(y+size_y > dimensions[1]-2 || y-size_y <= 2) {
        cerr << "The cylinder must completely lie inside the dimensions of the HDF file. Please check the parameters size_y and offset_y." << endl;
        exit(1);
      }
    }
    if(direction != "z") {
      size_z = atof(config->getValue("size_z").c_str());
      if(z+size_z > dimensions[2]-2 || z-size_z <= 2) {
        cerr << "The cylinder must completely lie inside the dimensions of the HDF file. Please check the parameters size_z and offset_z." << endl;
        exit(1);
      }
    }
    
    if(direction == "x") row = hdf->getCylinderRow(0, y, z, size_y, size_z);
    else if(direction == "y") row = hdf->getCylinderRow(1, z, x, size_z, size_x);
    else if(direction == "z") row = hdf->getCylinderRow(2, x, y, size_x, size_y);
  }
  else if(config->getValue("mode") == "single") {
    if(direction == "x") row = hdf->getRow(0, y, z);
    else if(direction == "y") row = hdf->getRow(1, z, x);
    else if(direction == "z") row = hdf->getRow(2, x, y);
  }
  //ToDo: implement error if no valid mode is supplied

    
  string filename = streak_dirname + "/streak.txt";
  Output out(filename);
  for(int i=0; i<row.at(0).size(); i++) {
    out << i;
    out << " ";
    out << row.at(0).at(i);
    out << " ";
    out << row.at(1).at(i);
    out << "\n";
  }
  
  return;
};

void Controller::plotSlice(const string datafile, const int number) const {
#ifndef _WIN32
  string title, xlabel, ylabel;
  int xrange, yrange;
  if(direction == "x") {
    title = "yz-plane";
    xlabel = "y";
    ylabel = "z";
    xrange = dimensions.at(1);
    yrange = dimensions.at(2);
  }
  else if(direction == "y") {
    title = "xz-plane";
    xlabel = "z";
    ylabel = "x";
    xrange = dimensions.at(2);
    yrange = dimensions.at(0);
  }
  else if(direction == "z") {
    title = "xy-plane";
    xlabel = "x";
    ylabel = "y";
    xrange = dimensions.at(0);
    yrange = dimensions.at(1);
  }
  
  Gnuplot::set_terminal_std("png");
  Gnuplot g1("lines");
  g1.cmd("set terminal png size 1500,1500 24");
  g1.cmd("set output '" + datafile + ".png'");
  g1.cmd("set pm3d map");
 // g1.cmd("set margin 0");
  g1.cmd("set title '" + title + ", " + direction + " = " + intToString(number) + "'");
  g1.cmd("set xlabel '" + xlabel + "'");
  g1.cmd("set ylabel '" + ylabel + "'");
  g1.cmd("set xrange [0:" + intToString(xrange) + "]");
  g1.cmd("set yrange [0:" + intToString(yrange) + "]");
  g1.cmd("set cbrange [0:" + config->getValue("cbrange") + "]");
  g1.cmd("splot '" + datafile + "' u 1:2:3 title ''");
#endif
};



string Controller::getLogFileName(const string prefix, const string ending) const {
  time_t rawtime;
  struct tm * ptm;
  time ( &rawtime );
  ptm = gmtime ( &rawtime );
  char buffer[50];
  sprintf (buffer,"%4d%02d%02d_%02d%02d%02d", (ptm->tm_year+1900), ptm->tm_mon+1, ptm->tm_mday,  (ptm->tm_hour)%24, ptm->tm_min, ptm->tm_sec);
  string timestring=buffer;
  string filename=prefix+"-"+timestring+"."+ending;
  return filename;
}
