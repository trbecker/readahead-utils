#include <stdlib.h>
#include <string.h>
#include "libparser.h"

struct config_entry *config_entry_new() {
	struct config_entry *new = malloc(sizeof(struct config_entry));
	if (!new) {
		return NULL; // Make this an err_ptr
	}

	memset(new, 0, sizeof(struct config_entry));
	list_init(&new->list);
	return new;
}


void config_entry_free(struct config_entry *ce)
{
#define sfree(ce) if (ce) free(ce)
	sfree(ce->mountpoint);
	sfree(ce->fstype);
#undef sfree
}
