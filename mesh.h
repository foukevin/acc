#ifndef MESH_H
#define MESH_H

#include "acc-mesh.h"
#include <stddef.h>

typedef double vec3[3];

struct generic_mesh {
	size_t numverts;
	vec3 *vertices;
	vec3 *normals;
};

void mesh_init(struct generic_mesh *mesh);

/* import */
int mesh_obj_read(struct accmesh *accmesh);

/* export */
int write_gl_mesh(struct accmesh *accmesh);

#endif /* MESH_H */
