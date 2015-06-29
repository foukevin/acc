#include "mesh.h"

void mesh_init(struct generic_mesh *mesh)
{
	mesh->verts = new_array(8192, sizeof(vec3));
	mesh->texcos = new_array(8192, sizeof(vec3));
	mesh->norms = new_array(8192, sizeof(vec3));
	mesh->polys = new_array(4096, sizeof(struct polygon));
}

struct triangle_mesh *mesh_to_triangle_mesh(const struct generic_mesh *mesh)
{
	struct triangle_mesh *new_trimesh = malloc(sizeof(*new_trimesh));

	int i;
	for (i = 0; i < mesh->polys->size; ++i) {
		const struct polygon *poly = array_get(mesh->polys, i);
		printf("Polygon size: %d\n", poly->verts->size);
	}
}

