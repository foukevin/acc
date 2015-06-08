#ifndef MESH_H
#define MESH_H

typedef struct {
	double x, y, z;
} vec3;

struct generic_mesh {
	vec3 *vertices;
	vec3 *normals;
};

#endif /* MESH_H */
