#ifndef MESH_H
#define MESH_H

#include "acc-mesh.h"
#include "array.h"

#include <stddef.h>

typedef double vec3[3];

struct vertex {
	size_t pos_index;
	size_t texco_index;
	size_t norm_index;
};

struct polygon {
	array_t *verts;
	int mat_index;
};

struct generic_mesh {
	array_t *verts;
	array_t *texcos;
	array_t *norms;
	array_t *polys;
};

/* TODO: needed? */
struct triangle {
	struct vertex verts[3];
};

struct triangle_mesh {
	array_t *attribs;
	array_t *surfs;
};

void mesh_init(struct generic_mesh *mesh);
struct triangle_mesh *mesh_to_triangle_mesh(const struct generic_mesh *mesh);

/* import */
int mesh_obj_read(struct accmesh *accmesh);

/* export */
int write_gl_mesh(struct accmesh *accmesh);

#endif /* MESH_H */
