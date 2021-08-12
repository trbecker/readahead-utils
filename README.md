# readahead-utils

Helper to set readahead based on filesystem characteristics.

## Usage
* Install the package by running.

```
make install
```

The build depends on 

* Edit the file `/etc/readahead.conf`, adding lines in the format below.
```
<type> [<entity>] <readahead>
```
  where  `type` is `mountpoint` or `default` and `readahead` is the value for the readahead.

  If `type` is `default`, `entity` must be omitted. This will be used if the mountpoint is not configured.

  If `type` is `mountpoint`, then `entity` must be the path to the mount point.

  The file is parsed top to bottom, stopping at the first match. No wildcard is available currntly.

### Sample configuration
```
default 128
mountpoint /mnt 2048
mountpoint /var 1024
```

## TODO
* Improve the parser.
* Handle error cases.
* Provide a proper build system (autoconf and such)
* Package (RPM, deb).
