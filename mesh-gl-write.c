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
	char name[16];
	uint32_t num_vert_attribs;
	uint32_t pad0;
	uint32_t num_surfs;
	uint32_t surf_desc_offset; /* surface descriptor offset */
	uint32_t vcount;
	uint32_t vdata_off;
	uint32_t vdata_sz;
	uint32_t icount;
	uint32_t idata_off;
	uint32_t idata_sz;
	float aabb_center[3];
	float aabb_extent[3];
};

int write_gl_mesh(struct accmesh *accmesh)
{
	struct filemap map;
	struct generic_mesh *mesh;
	struct glhdr *hdr;

	mesh = accmesh->mesh;

	mapfile(&map, accmesh->meshname, 1);
	char *const sof = map.buf; /* start of file */
	char *eof = sof;

	/* setup header */
	hdr = (struct glhdr *)sof;
	memset(hdr, 0, sizeof(*hdr));
	eof += sizeof(*hdr);

	/* vertices */
	{
		int i;

		/* append data after header */
		char *const data = eof;
		float *dst = (float *)data;
		for (i = 0; i < mesh->numverts; ++i) {
			const vec3 *v = &mesh->vertices[i];
			dst[0] = (float)(*v[0]);
			dst[1] = (float)(*v[1]);
			dst[2] = (float)(*v[2]);
			dst += 3;
		}
		eof = (char *)dst;

		hdr->vcount = mesh->numverts;
		hdr->vdata_off = data - sof;
		hdr->vdata_sz = eof - data;
	}

	/* indices */
	{
		int i;

		char *const data = eof;
		float *dst = (uint16_t *)data;
	}

	unmapfile(&map, eof - sof);
	return 0;
}
