#include "mesh.h"

void mesh_init(struct generic_mesh *mesh)
{
	mesh->numverts = 0;
	mesh->vertices = malloc(sizeof(vec3) * 32768);
	mesh->numtris = 0;
	mesh->triangles = malloc(sizeof(struct triangle) * 32768);
}

double *mesh_get_vertex(struct generic_mesh *mesh)
{
	return &mesh->vertices[mesh->numverts++];
}

struct triangle *mesh_get_triangle(struct generic_mesh *mesh)
{
	return &mesh->triangles[mesh->numtris++];
}

