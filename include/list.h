#ifndef __ra_list_h__
#define __ra_list_h__
struct list_head
{
	struct list_head *next, *prev;
};

static inline void list_init(struct list_head *lh)
{
	lh->next = lh;
	lh->prev = lh;
}

#define LIST_DECLARE(name)	 \
	struct list_head name;	 \
	list_init(&name);

static inline void list_add(struct list_head *l, struct list_head *a)
{
	a->prev = l;
	a->next = l->next;
	l->next->prev = a;
	l->next = a;
}

static inline void list_del(struct list_head *a)
{
	a->next->prev = a->prev;
	a->prev->next = a->next;
	a->next = a;
	a->prev = a;
}

#define list_for_each(pos, head)				\
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_free(head, free_func) ({				\
	for (struct list_head *__lh = (head)->next;		\
			__lh != head; __lh = (head)->next) {	\
		list_del(__lh);					\
		free_func(__lh);				\
	}})							

#define containerof(p, type, field) ({				\
	const __typeof__(((type *)0)->field) *__ptr = (p);	\
	(type *)((char *)__ptr - offsetof(type, field) ); })

#endif
