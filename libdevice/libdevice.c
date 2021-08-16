#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libmount/libmount.h>
#include <sys/sysmacros.h>

#include "list.h"
#include "libdevice.h"

static dev_t dev_from_arg(const char *device_number)
{
	char *s = strdup(device_number), *p;
	char *maj_s, *min_s;
	unsigned int maj, min;
	dev_t dev;

	maj_s = p = s;
	for ( ; *p != ':'; p++)
		;

	*p = '\0';
	min_s = p + 1;

	maj = strtol(maj_s, NULL, 10);
	min = strtol(min_s, NULL, 10);

	dev = makedev(maj, min);

	free(s);
	return dev;
}

static void init_device_info(struct device_info *di, const char *device_number)
{
	di->device_number = strdup(device_number);
	di->dev = dev_from_arg(device_number);
	di->mountpoint = NULL;
	di->fstype = NULL;
}

void free_device_info(struct device_info *di)
{
	if (di->mountpoint)
		free(di->mountpoint);
	if (di->fstype)
		free(di->fstype);
	if (di->device_number)
		free(di->device_number);
}

int get_mountinfo(const char *device_number, struct device_info *device_info, const char *mountinfo_path)
{
	int ret = 0;
	struct libmnt_table *mnttbl;
	struct libmnt_fs *fs;
	char *target;

	init_device_info(device_info, device_number);

	mnttbl = mnt_new_table();

	if ((ret = mnt_table_parse_file(mnttbl, mountinfo_path)) < 0)
		goto out_free_tbl;

	if ((fs = mnt_table_find_devno(mnttbl, device_info->dev, MNT_ITER_FORWARD)) == NULL) {
		ret = ENOENT;
		goto out_free_tbl;
	}

	if ((target = (char *)mnt_fs_get_target(fs)) == NULL) {
		ret = ENOENT;
		goto out_free_fs;
	}

	device_info->mountpoint = strdup(target);
	target = (char *)mnt_fs_get_fstype(fs);
	if (target)
		device_info->fstype = strdup(target);

out_free_fs:
	mnt_free_fs(fs);
out_free_tbl:
	mnt_free_table(mnttbl);
	free(device_info->device_number);
	device_info->device_number = NULL;
	return ret;
}
