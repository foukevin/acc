#include "mapfile.h"
#include "error.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * mode 0 read, 1 write
 */
void mapfile(struct filemap *map, const char *filename, int mode)
{
	/* memory map input file */
	char *buf;
	struct stat s;
	int fd;

	if (mode)
		fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(filename, O_RDONLY);

	if (fd < 0)
		error("opening '%s'", filename);
	fstat(fd, &s);

	size_t size = mode ? 1024 * 1024 : s.st_size; /* 1Mb max */
	int prot = mode ? PROT_WRITE : PROT_READ;
	int flags = mode ? MAP_SHARED : MAP_PRIVATE;

  	buf = mmap(0, size, prot, flags, fd, 0);
	if (buf == MAP_FAILED) {
		close(fd);
		error("mapping '%s'", filename);
	}

	map->fd = fd;
	map->buf = buf;
	map->size = size;
}

void unmapfile(struct filemap *map)
{
	munmap(map->buf, map->size);
	close(map->fd);
}

