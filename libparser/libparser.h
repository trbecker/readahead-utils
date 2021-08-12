#ifndef __ra_libparser_h__
#define __ra_libparser_h__
#include "list.h"

struct config_entry {
	struct list_head list;
	char *mountpoint;
	char *fstype;
	int readahead;
};

extern int parse_config(const char *, struct list_head *config_list);
#endif
