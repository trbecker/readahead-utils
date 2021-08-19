# readahead-utils

Helper to set readahead based on filesystem characteristics.

## Usage
* Install the package by running.

```
./autogen.sh
./configure
make
make install

```

The configuration file is composed of matches which are written in the form.
```
<(match>+|default) readahead=<value>
```

The file is line oriented, each line representing a match.

A match is defined as a attribute to match to a value.
```
<attribute>=<value>
fstype=nfs
```

Currently it is posible to match for `mountpoint` and `fstype`. The first
is a path in the system where the file system is mounted, and the second is
a file system type as defined in /proc/self/mountinfo.

### Example configuration
```
default					readahead=128
mountpoint=/mnt		fstype=nfs	readahead=16384
			fstype=nfs	readahead=7864320
			fstype=nfs4	readahead=7864320
```

## TODO
* Improve the parser.
  * Add comment lines
  * Allow empty lines at the beginning of the file
* Handle error cases.
* deb packaging
* man pages
* debugging
  * allow to turn on/off
  * allow to chose the output device for debugging
