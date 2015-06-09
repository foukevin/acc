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

int main(int argc, char **argv)
{
	struct accmesh *accmesh, accmesh_storage;

	accmesh = &accmesh_storage;
	memset(accmesh, 0, sizeof(*accmesh));

	ERROR_INIT;

	accmesh = &accmesh_storage;
	memset(accmesh, 0, sizeof(*accmesh));

	if (*argv == NULL) {
		accmesh->progname = "accmesh";
	} else {
		accmesh->progname = *argv;
	}

	accmesh->argc = argc;
	accmesh->argv = argv;
	parse_options(accmesh);

	/* memory map input file */
	char *buffer;
	struct stat s;
	int fd = open(accmesh->filename , O_RDONLY);
	if (fd < 0)
		error("opening '%s'", accmesh->filename);
	fstat(fd, &s);
	/* PROT_READ disallows writing to buffer: will segv */
  	buffer = mmap(0, s.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buffer == MAP_FAILED) {
		close(fd);
		error("reading '%s'", accmesh->filename);
	}

	struct generic_mesh *mesh, mesh_storage;
	mesh = &mesh_storage;
	mesh_obj_read(mesh, buffer, s.st_size);
	printf("vertex count: %d\n", mesh->numverts);

	munmap(buffer, s.st_size);
	close(fd);
	return accmesh->return_value;
}
