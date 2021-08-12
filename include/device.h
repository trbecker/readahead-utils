#ifndef __ra_device_h__
#define __ra_device_h__
struct device_info {
	char *device_number;
	dev_t dev;
	char *mountpoint;
	char *fstype;
};
#endif
