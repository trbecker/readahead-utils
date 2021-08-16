#ifndef __ra_device_h__
#define __ra_device_h__
struct device_info {
	char *device_number;
	dev_t dev;
	char *mountpoint;
	char *fstype;
};

extern int get_mountinfo(const char *, struct device_info *, const char *);
extern void free_device_info(struct device_info *);
#endif
