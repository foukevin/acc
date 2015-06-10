#include "mesh.h"
#include "acc-mesh.h"
#include "error.h"

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

struct glhdr {
	uint32_t numverts;
};

int write_gl_mesh(struct accmesh *accmesh)
{
	int fd;
	struct generic_mesh *mesh;
	struct glhdr *hdr, hdr_storage;
	hdr = &hdr_storage;

	mesh = accmesh->mesh;
	fd = open(accmesh->meshname, O_WRONLY | O_CREAT | O_TRUNC, 0660);
	if (fd == -1)
		error("opening '%s'", accmesh->meshname);

	hdr->numverts = mesh->numverts;
	write(fd, hdr, sizeof(*hdr));

	close(fd);
	return 0;
}
