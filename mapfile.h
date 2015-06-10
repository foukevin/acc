#ifndef MAPFILE_H
#define MAPFILE_H

#include <stddef.h>

struct filemap {
	int fd;
	void *buf;
	size_t size;
};

void mapfile(struct filemap *map, const char *filename, int mode);
void unmapfile(struct filemap *map);

#endif /* MAPFILE_H */
