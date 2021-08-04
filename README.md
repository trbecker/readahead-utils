# readahead-utils

Helper to set readahead based on filesystem characteristics.

## Usage
* Install the package by running

```
make install
```

* Edit the file `/etc/readahead.conf`, adding lines in the format below.
```
<type> [<entity>] <readahead>
```
where  `type` is `mountpoint` or `default` and `readahead` is the value for the readahead.

If `type` is `default`, `entity` must be omitted.

If `type` is `mountpoint`, then `entity` must be the path to the mount point.

### Sample configuration
```
default 128
mountpoint /mnt 2048
mountpoint /var 1024
```
