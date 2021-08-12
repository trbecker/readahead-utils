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
