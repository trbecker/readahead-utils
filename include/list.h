#ifndef __ra_list_h__
#define __ra_list_h__
struct list_head
{
	struct list_head *next, *prev;
};

static void list_init(struct list_head *lh)
{
	lh->next = lh;
	lh->prev = lh;
}

#define LIST_DECLARE(name)	 \
	struct list_head name;	 \
	list_init(&name);

static void list_add(struct list_head *l, struct list_head *a)
{
	a->prev = l;
	a->next = l->next;
	l->next->prev = a;
	l->next = a;
}

static void list_del(struct list_head *a)
{
	a->next->prev = a->prev;
	a->prev->next = a->next;
	a->next = a;
	a->prev = a;
}
#endif
