#include "mesh.h"
#include "acc-mesh.h"
#include "error.h"
#include "mapfile.h"

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct glhdr {
	uint32_t numverts;
};

int write_gl_mesh(struct accmesh *accmesh)
{
	int i;
	struct filemap map;
	struct generic_mesh *mesh;
	struct glhdr *hdr, hdr_storage;
	hdr = &hdr_storage;

	mesh = accmesh->mesh;

	mapfile(&map, accmesh->meshname, 1);
	void *p = map.buf;
	ftruncate(map.fd, 512);

	hdr->numverts = mesh->numverts;
	memcpy(p, hdr, sizeof(*hdr));

	float *dst = p + sizeof(*hdr);
	for (i = 0; i < mesh->numverts; ++i) {
		const vec3 *v = &mesh->vertices[i];
		dst[0] = (float)(*v[0]);
		dst[1] = (float)(*v[1]);
		dst[2] = (float)(*v[2]);
		dst += 3;
	}

	unmapfile(&map);
	return 0;
}
