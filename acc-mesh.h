#ifndef ACC_MESH_H
#define ACC_MESH_H

struct accmesh {
	char *filename;
	char *meshname; /* output file name */
	int option_verbose;

	struct generic_mesh *mesh;

	/* State information */
	const char *progname;
	char **argv;
	int argc;
	int return_value;
};

#endif /* ACC_MESH_H */
