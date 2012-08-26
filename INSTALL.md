# On Linux / Mac OS

For compiling you need a C++ compiler (preferably the gcc), as well as the [boost libraries](http://www.boost.org). For graphical output [gnuplot](http://www.gnuplot.info) has to be installed.

You also have to have the HDF libraries install. You can obtain them from the [HDFgroup](http://www.hdfgroup.org/HDF5/release/obtain5.html).

## On Ubuntu:
Note that on Ubuntu it is not necessary to download and install the HDF5-libraries manually, since they are contained in the systems default software repository. Also all the other dependencies can be installed via *apt*. Please make sure the following packages - all from the default repositories - are install via *apt*:

* build-essential
* cmake
* gnuplot
* libboost-filesystem-dev, libboost-program-options-dev, libboost-system-dev
* libhdf5-serial-dev

Then run the following commands:

    cmake .
    make
    sudo make install


# On Windows
Please download the pre-compiled binary in the Downloads section.