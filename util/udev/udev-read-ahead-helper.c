#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

#include <libmount/libmount.h>

#include <sys/sysmacros.h>
#include "device.h"
#include "list.h"
#include "libparser.h"

#ifndef MOUNTINFO_PATH
#define MOUNTINFO_PATH "/proc/self/mountinfo"
#endif

#ifndef READAHEAD_CONFIG_FILE
#define READAHEAD_CONFIG_FILE "/etc/readahead.conf"
#endif

#define LINE_LEN 1024
#define READAHEAD_LINE_LINE_LENGHT 2048

static dev_t dev_from_arg(const char *);

static void init_device_info(struct device_info *di, const char *device_number)
{
	di->device_number = strdup(device_number);
	di->dev = dev_from_arg(device_number);
	di->mountpoint = NULL;
	di->fstype = NULL;
}

static void free_device_info(struct device_info *di)
{
	if (di->mountpoint)
		free(di->mountpoint);
	if (di->device_number)
		free(di->device_number);
}

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

static int get_mountinfo(struct device_info *device_info)
{
	int ret = 0;
	struct libmnt_table *mnttbl;
	struct libmnt_fs *fs;
	char *target;


	mnttbl = mnt_new_table();

	if ((ret = mnt_table_parse_file(mnttbl, MOUNTINFO_PATH)) < 0)
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

out_free_fs:
	mnt_free_fs(fs);
out_free_tbl:
	mnt_free_table(mnttbl);
	free(device_info->device_number);
	device_info->device_number = NULL;
	return ret;
}

static int get_readahead(struct device_info *di, int *readahead)
{
#define STRCMP(a, b) ((a) != NULL && (b) != NULL && strcmp((a), (b)) == 0)
	LIST_DECLARE(configs);
	struct list_head *lh;
	int ret = 0;
	int default_ra = 0;

	if ((ret = parse_config(READAHEAD_CONFIG_FILE, &configs)) != 0) {
		fprintf(stderr, "Failed to read configuration (%d)\n", ret);
		goto out_free_configs;
	}

	for (lh = configs.next; lh != &configs; lh = lh->next) {
		struct config_entry *ce =
		       (struct config_entry *)(lh - offsetof(struct config_entry, list));
		if (ce->mountpoint == NULL && ce->fstype == NULL) {
			default_ra = ce->readahead;
			continue;
		}

		if (STRCMP(ce->mountpoint, di->mountpoint) &&
				STRCMP(ce->fstype, di->fstype)) {
			*readahead = ce->readahead;
			goto out_free_configs;
		}

		if (STRCMP(ce->fstype, di->fstype)) {
			*readahead = ce->readahead;
			goto out_free_configs;
		}

		if (STRCMP(ce->mountpoint, di->mountpoint)) {
			*readahead = ce->readahead;
			goto out_free_configs;
		}
	}

	/* fallthrough */
	*readahead = default_ra;

out_free_configs:
	lh = configs.next;
	while (lh != &configs) {
		list_del(lh);
		free(lh);
		lh = configs.next;
	}

out:
	return ret;
#undef STRCMP
}

static int get_device_info(const char *device_number, struct device_info *device_info)
{
	int ret = 0;
	init_device_info(device_info, device_number);
	return get_mountinfo(device_info);
}


int main(int argc, char **argv)
{
	int ret, readahead = 0;
	struct device_info device_info;

	if ((ret = get_device_info(argv[1], &device_info)) != 0)
		goto out_free;
	if ((ret = get_readahead(&device_info, &readahead)) != 0)
		goto out_free;

	printf("%d\n", readahead);

out_free:
	free_device_info(&device_info);
out:
	return ret;
}
