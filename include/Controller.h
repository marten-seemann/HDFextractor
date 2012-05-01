#ifndef HDF_Controller_h
#define HDF_Controller_h

#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <ctime>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>


#include "gnuplot_cpp/gnuplot_i.h"

#include "Config.h"
#include "Output.h"
#include "HDFCryst.h"
#include "functions.h"



/**
 * @brief A class for controlling the flow of the programm
 *
 * This class handles the input via command line and calls all the methods of the other classes to generate the output the user expect
 * 
 */

class Controller {
private:
  Config* config;
  HDFCryst* hdf;
  /// directory index: 0 for @a x, 1 for @a y, 2 for @a z
  /** determined from #direction */
  int dir_index;
  /// the number of the run
  /** possible values: 1 or 2 */
  int run;
  /// the direction
  /** possible values: x, y or z */
  std::string direction;
  /// the path to the output directory
  std::string output_dirname;
  /// path of the output directory for the streak files
  /** will only be set if #run is 2 */
  std::string streak_dirname; // output directory for the streak files
  std::vector<int> dimensions;
  
public:
  /// constructor
  /// @param argc the @a argc as defined in the main() function
  /// @param argv the @a argv as defined in the main() function
  explicit Controller(int argc, const char * argv[]);
  /// destructor
  ~Controller();
  
private:
  /// initiate the calculations according to the options specified in the configuration file
  /** creates a new instance of ::Config with the configuration file whose path is given via command line parameters
    * @param filename the path to the config file
   */
  void readConfigFile(const std::string filename);
  /// show general information about the hdf file
  /** prints dimension, the UBI matrix as well as the NP (number of points) value
   *  data is read from the header of the HDF file
   */
  void printInfos() const;
  /// open a HDF file
  /** calls the class %HDFCryst for opening the specified HDF file
   * @param filename the path to the HDF file
   */
  void openHDF(const std::string filename);
  /// create the output directories
  /** create the whole output directory structure. This structure depends on the value of #run (and the value of #direction) */
  void createOutputDirs();
  /// get the #dir_index from the string @a x, @a y or @a z
  void getDirection();
  /// perform run 1 (extraction of the layers)
  void doRun1();
  /// perform run 2 (extraction of the streaks)
  void doRun2();
  /// plot a slice to a .png file
  /** calls Gnuplot to plot a data file and saves it as a .png file to the output directory
   *  @param datafile the file containing the data which should be plotted
   *  @param number the number of the slice that is plotted in the specified direction. will only be used in the plot title
   */
  void plotSlice(const std::string datafile, const int number) const;
  /// assemble the filename of the logfile
  /** The log file will be saved to the output directory.
   *  @param prefix a prefix for the file name
   *  @param ending the file suffix
   *  @return the assembled filename of the logfile
   */
  std::string getLogFileName(const std::string prefix, const std::string ending) const;
  
};


#endif
