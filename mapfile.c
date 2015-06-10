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
	int fd = open(filename , mode ? O_WRONLY : O_RDONLY);
	if (fd < 0)
		error("opening '%s'", filename);
	fstat(fd, &s);

  	buf = mmap(0, s.st_size, mode ? PROT_WRITE : PROT_READ, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		close(fd);
		error("mapping '%s'", filename);
	}

	map->fd = fd;
	map->buf = buf;
	map->size = s.st_size;
}

void unmapfile(struct filemap *map)
{
	munmap(map->buf, map->size);
	close(map->fd);
}

