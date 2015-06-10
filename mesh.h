#ifndef MESH_H
#define MESH_H

#include "acc-mesh.h"
#include <stddef.h>

typedef struct {
	double x, y, z;
} vec3;

struct generic_mesh {
	size_t numverts;
	vec3 *vertices;
	vec3 *normals;
};

/* import */
int mesh_obj_read(struct generic_mesh *mesh, const char *obj, size_t size);

/* export */
int write_gl_mesh(struct accmesh *accmesh);

#endif /* MESH_H */
