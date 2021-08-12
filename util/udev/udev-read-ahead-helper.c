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

#ifndef MOUNTINFO_PATH
#define MOUNTINFO_PATH "/proc/self/mountinfo"
#endif

#ifndef READAHEAD_CONFIG_FILE
#define READAHEAD_CONFIG_FILE "/etc/readahead.conf"
#endif

#define LINE_LEN 1024
#define READAHEAD_LINE_LINE_LENGHT 2048

static dev_t dev_from_arg(const char *);

struct device_info {
	char *device_number;
	dev_t dev;
	char *mount_point;
	unsigned int read_ahead;
};

static void init_device_info(struct device_info *di, const char *device_number)
{
	di->device_number = strdup(device_number);
	di->dev = dev_from_arg(device_number);
	di->mount_point = NULL;
	di->read_ahead = 0;
}

static void free_device_info(struct device_info *di)
{
	if (di->mount_point)
		free(di->mount_point);
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

	device_info->mount_point = strdup(target);

out_free_fs:
	mnt_free_fs(fs);
out_free_tbl:
	mnt_free_table(mnttbl);
	free(device_info->device_number);
	device_info->device_number = NULL;
	return ret;
}

static int get_readahead(struct device_info *di)
{
	int default_readahead;
	FILE *fp;
	int ret = 0;
	char *line;
	size_t n;

	if ((fp = fopen(READAHEAD_CONFIG_FILE, "r")) == NULL) {
		ret = errno;
		goto out;
	}

	line = malloc(READAHEAD_LINE_LINE_LENGHT);
	if (!line) {
		ret = ENOMEM;
		goto out_close;
	}

	n = READAHEAD_LINE_LINE_LENGHT;
	errno = 0;
	while ((ret = getline(&line, &n, fp)) > 0) {
		char *p, *type = NULL, *entity = NULL, *readahead = NULL;


		/* 
		 * Line format: type entity readahead
		 * type can be mountpoint
		 * entity is the path to the mountpoint
		 * readahead is the readahead value
		 * Exception:
		 *   - one line contains the default value in the format
		 *     default <readahead>
		 */
		type = line;
		for (p = line; p < line + n; p++) {
			if (*p == ' ') {
				*p = '\0';
				if (!entity) {

					if (strcmp("default", type) == 0) {
						default_readahead = strtol(p + 1, NULL ,10);
						break;
					} else
						entity = ++p;
					continue;
				}

				if (!readahead) {
					readahead = p + 1;
					break;
				}
			}
		}

		if (strcmp("mountpoint", type) == 0 &&
				strcmp(entity, di->mount_point) == 0) {
			di->read_ahead = strtol(readahead, NULL, 10);
			break;
		}

		n = READAHEAD_LINE_LINE_LENGHT;
		errno = 0;
	}

	if (errno) {
		ret = errno;
	} else {
		ret = 0;
		if (!di->read_ahead)
			if (default_readahead) {
				di->read_ahead = default_readahead;
			} else {
				ret = ENOENT;
			}
	}

out_free:
	free(line);
out_close:
	fclose(fp);
out:
	return ret;
}

static int get_device_info(const char *device_number, struct device_info *device_info)
{
	int ret = 0;
	init_device_info(device_info, device_number);
	if ((ret = get_mountinfo(device_info)) != 0)
		return ret;

	return get_readahead(device_info);
}


int main(int argc, char **argv)
{
	int ret;
	struct device_info device_info;

	if ((ret = get_device_info(argv[1], &device_info)) == 0)
		printf("%d\n", device_info.read_ahead);

	free_device_info(&device_info);
	return ret;
}
