#include "mesh.h"
#include "acc-mesh.h"
#include "error.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h> /* exit */
#include <string.h> /* memset */

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
	return getopt_long(accmesh->argc, accmesh->argv, "hvo:", long_options, NULL);
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
		case 'o':
			accmesh->meshname = strdup(optarg);
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

	if (!accmesh->meshname) {
		error("no output file");
		exit(1);
	}

	if (accmesh->option_verbose) {
		printf("input file: %s\n", accmesh->filename);
		printf("output file: %s\n", accmesh->meshname);
	}
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

	mesh_obj_read(accmesh);
	write_gl_mesh(accmesh);

	return accmesh->return_value;
}
