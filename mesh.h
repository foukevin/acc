#ifndef MESH_H
#define MESH_H

#include <stddef.h>

typedef struct {
	double x, y, z;
} vec3;

struct generic_mesh {
	size_t numverts;
	vec3 *vertices;
	vec3 *normals;
};

int mesh_obj_read(struct generic_mesh *mesh, const char *obj, size_t size);

#endif /* MESH_H */
