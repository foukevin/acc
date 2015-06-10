#include "mesh.h"
#include "acc-mesh.h"
#include "error.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memset */

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static void usage(const struct accmesh *accmesh)
{
	fprintf(stderr, "Usage: %s [options] mesh\n", accmesh->progname);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  --help                   Display this information\n");
	exit(0);
}

static struct option long_options[] = {
	{ "help", no_argument, 0, 'h' },
	{ "verbose", no_argument, 0, 'v' },
	{ 0, 0, 0, 0 }
};

static int accmesh_getopt(struct accmesh *accmesh)
{
	return getopt_long(accmesh->argc, accmesh->argv, "hv", long_options, NULL);
}

void parse_options(struct accmesh *accmesh)
{
	int opt;
	int invalid_arg = 0;

	while ((opt = accmesh_getopt(accmesh)) != -1) {
		switch (opt) {
		case 'h':
			usage(accmesh);
			break;
		case 'v':
			accmesh->option_verbose = 1;
			break;
		case '?':
		default:
			error("invalid option: '%c'", optopt);
			break;
		}

		if (invalid_arg)
			exit(1);
	}

	/* Handle non-option arguments (ie: input file names) */
	if (optind < accmesh->argc) {
		/*
		 * Take the first positional argument, pending ones
		 * are just ignored.
		 */
		accmesh->filename = strdup(accmesh->argv[optind++]);
	}

	if (!accmesh->filename) {
		error("no input file");
		exit(1);
	}

	if (accmesh->option_verbose) {
		printf("input file: %s\n", accmesh->filename);
	}
}

struct filemap {
	int fd;
	void *buf;
	size_t size;
};

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

int main(int argc, char **argv)
{
	struct generic_mesh mesh_storage;
	struct accmesh *accmesh, accmesh_storage;

	accmesh = &accmesh_storage;
	memset(accmesh, 0, sizeof(*accmesh));

	ERROR_INIT;

	accmesh = &accmesh_storage;
	memset(accmesh, 0, sizeof(*accmesh));

	accmesh->progname = (*argv != NULL) ? *argv : "accmesh";
	accmesh->argc = argc;
	accmesh->argv = argv;
	accmesh->mesh = &mesh_storage;
	parse_options(accmesh);

	struct filemap map;
	mapfile(&map, accmesh->filename, 0);
	mesh_obj_read(accmesh->mesh, map.buf, map.size);
	unmapfile(&map);

	accmesh->meshname = "/tmp/untitled.bin";
	write_gl_mesh(accmesh);

	return accmesh->return_value;
}
