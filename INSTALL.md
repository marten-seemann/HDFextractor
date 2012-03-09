# On Linux

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


# On Windows XP (32 bit):
not (yet) completely tested. Try on your on risk. You will save much time switching to a Linux system...

* download CMake from [http://www.cmake.org/cmake/resources/software.html](http://www.hdfgroup.org/HDF5/release/obtain5.html)
* install http://www.mingw.org/, enable C++, MSys Basic System and MinGW Developer Toolkit
* add C:\MinGW\bin to the Path (Path, NOT PATH) variable, see [http://www.computerhope.com/issues/ch000549.htm](http://www.computerhope.com/issues/ch000549.htm)
* boost???

If you succeed in installing on Windows, please contact me. I will update the instructions here. Thank you!