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
	int i;
	int fd;
	struct generic_mesh *mesh;
	struct glhdr *hdr, hdr_storage;
	float glvec3[3];
	hdr = &hdr_storage;

	mesh = accmesh->mesh;
	fd = open(accmesh->meshname, O_WRONLY | O_CREAT | O_TRUNC, 0660);
	if (fd == -1)
		error("opening '%s'", accmesh->meshname);

	hdr->numverts = mesh->numverts;
	write(fd, hdr, sizeof(*hdr));

	for (i = 0; i < mesh->numverts; ++i) {
		const vec3 *v = &mesh->vertices[i];
		glvec3[0] = (float)(*v[0]);
		glvec3[1] = (float)(*v[1]);
		glvec3[2] = (float)(*v[2]);
		write(fd, glvec3, sizeof(glvec3));
	}

	close(fd);
	return 0;
}
