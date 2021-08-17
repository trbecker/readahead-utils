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

#include "log.h"
#include "list.h"
#include "libparser.h"
#include "libdevice.h"

#ifndef MOUNTINFO_PATH
#define MOUNTINFO_PATH "/proc/self/mountinfo"
#endif

#ifndef READAHEAD_CONFIG_FILE
#define READAHEAD_CONFIG_FILE "/etc/readahead.conf"
#endif

#define LINE_LEN 1024
#define READAHEAD_LINE_LINE_LENGHT 2048

static void debug_config_entry(struct config_entry *ce)
{
	debug("config_entry: mountpoint = %s, fstype = %s, readahead = %d",
		ce->mountpoint, ce->fstype, ce->readahead);

}

static void debug_config_entries(struct list_head *ces)
{
	struct list_head *lh;

	list_for_each(lh, ces) {
		struct config_entry *ce = containerof(lh, struct config_entry, list);
		debug_config_entry(ce);
	}
}

static int match_config(struct device_info *di, struct config_entry *ce) {
#define FIELD_CMP(field) \
	(ce->field == NULL || (di->field != NULL && strcmp(di->field, ce->field) == 0))

	if (!FIELD_CMP(mountpoint))
		return 0;

	if (!FIELD_CMP(fstype))
		return 0;

	debug("Device matched with config\n");
	return 1;
#undef STRCMP
}

static void config_entry_list_head_free(struct list_head *lh) {
	struct config_entry *ce = containerof(lh, struct config_entry, list);
	config_entry_free(ce);
}

static int get_readahead(struct device_info *di, int *readahead)
{
	LIST_DECLARE(configs);
	struct list_head *lh;
	int ret = 0;
	int default_ra = 0;

	if ((ret = parse_config(READAHEAD_CONFIG_FILE, &configs)) != 0) {
		err("Failed to read configuration (%d)\n", ret);
		goto out_free_configs;
	}

	debug_config_entries(&configs);

	list_for_each(lh, &configs) {
		struct config_entry *ce = containerof(lh, struct config_entry, list);
		if (ce->mountpoint == NULL && ce->fstype == NULL) {
			default_ra = ce->readahead;
			continue;
		}

		if (match_config(di, ce)) {
			*readahead = ce->readahead;
			goto out_free_configs;
		}
	}

	/* fallthrough */
	debug("Setting readahead to default %d\n", default_ra);
	*readahead = default_ra;

out_free_configs:
	list_free(&configs, config_entry_list_head_free);
out:
	return ret;
}

static int get_device_info(const char *device_number, struct device_info *device_info)
{
	return get_mountinfo(device_number, device_info, MOUNTINFO_PATH);
}

int main(int argc, char **argv)
{
	int ret, readahead = 0;
	struct device_info device_info;

	log_open();

	if ((ret = get_device_info(argv[1], &device_info)) != 0) {
		err("Failed to find device (%d)\n", ret);
		goto out_free;
	}

	if ((ret = get_readahead(&device_info, &readahead)) != 0) {
		err("Failed to find readahead (%d)\n", ret);
		goto out_free;
	}

	info("Setting read ahead for bdi %s to %d\n", argv[1], readahead);

	printf("%d\n", readahead);

out_free:
	free_device_info(&device_info);
out:
	log_close();
	debug("exiting with code %d\n", ret);
	return ret;
}
