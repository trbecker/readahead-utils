#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "libparser.h"
#include "list.h"

int main(int argc, char **argv)
{
	LIST_DECLARE(entries);
	printf("Parsing config file\n");
	parse_config(argv[1], &entries);
	printf("Parsed\n");

	for (struct list_head *entry = entries.next ; entry != &entries ; entry=entry->next) {
		struct config_entry *ce = (struct config_entry *)(entry + offsetof(struct config_entry, list));
		printf("parsed entry: mp = %s, fstype = %s, ra = %d\n", ce->mountpoint, ce->fstype, ce->readahead);
	}

	while (entries.next != &entries) {
		struct config_entry *ce = (struct config_entry *)(entries.next + offsetof(struct config_entry, list));
		list_del(entries.next);
		free(ce);
	}

	return 0;
}
	
