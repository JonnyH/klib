#ifndef __KCONTAINER_OF
#define __KCONTAINER_OF

#define kcontainer_of(ptr, type, member) ({ \
	(type *)((char*)ptr - offsetof(type,member));})

#endif /* __KCONTAINER_OF */

