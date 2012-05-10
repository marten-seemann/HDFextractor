# Usage

In the following it is assumed that the file name of the HDF file is *example.hdf*.

## Obtain general information about the DHF file

Print general information, such as the resolution (NP value) or the UBI matrix.

	HDFextractor --show-info --hdf example.hdf
	
## Start Run 1
An example configuration file is provided in the folder examples/. For further explaination see there.

	HDFextractor --config run1.cfg

## Start Run 2
An example configuration file is provided in the folder examples/. For further explaination see there.

	HDFextractor --config run2.cfg
