
struct accmesh {
	char *filename;
	int option_verbose;

	/* State information */
	const char *progname;
	char **argv;
	int argc;
	int return_value;
};

