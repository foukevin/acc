#ifndef MESH_H
#define MESH_H

#include "acc-mesh.h"
#include <stddef.h>

typedef double vec3[3];

struct triangle {
	int flags;
	size_t verts[3];
	size_t normals[3];
};

struct generic_mesh {
	size_t numverts;
	vec3 *vertices;
	size_t numnormals;
	vec3 *normals;
	size_t numtris;
	struct triangle *triangles;
};

void mesh_init(struct generic_mesh *mesh);

double *mesh_get_vertex(struct generic_mesh *mesh);
struct triangle *mesh_get_triangle(struct generic_mesh *mesh);

/* import */
int mesh_obj_read(struct accmesh *accmesh);

/* export */
int write_gl_mesh(struct accmesh *accmesh);

#endif /* MESH_H */
