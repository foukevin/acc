#include "mesh.h"
#include "parse.h"
#include "mapfile.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h> /* memcpy */

enum obj_token_type {
	OT_COMMENT,
	OT_OBJECT,
	OT_VERTEX,
	OT_VERTEX_NORMAL,
	OT_FACE,
	OT_UNKNOWN,
};

static const struct obj_token tokens[] = {
	{ "#", OT_COMMENT },
	{ "o", OT_OBJECT },
	{ "v", OT_VERTEX },
	{ "vn", OT_VERTEX_NORMAL },
	{ "f", OT_FACE },
	{ NULL, OT_UNKNOWN },
};

static int parse_line(const char *linep, size_t linelen,
		      struct generic_mesh *mesh)
{
	int i;
	//size_t tokenlen = get_token(token, 256, linep, linelen);
	vec3 v;
	char *token, *string, *tofree;

	tofree = string = strdup(linep);
	assert(string != NULL);

	/* get identifier type */
	enum obj_token_type type = OT_UNKNOWN;
	{
		const struct obj_token *tokp;
		char *identifier = strsep(&string, " ");
		if (identifier != NULL)
			for (tokp = tokens; tokp->name != OT_UNKNOWN; ++tokp)
				if (!strcmp(tokp->string, identifier))
					type = tokp->name;
		printf("id: %s, type: %d ", identifier, type);
	}

	char *face;

	switch (type) {
	case OT_COMMENT:
		puts("comment");
		break;
	case OT_VERTEX:
		puts("vertex");
		for (i = 0; i < 3; ++i) {
			token = strsep(&string, " ");
			assert(token != NULL);
			v[i] = atof(token);
		}
		array_append(mesh->verts, &v);
		break;
	case OT_VERTEX_NORMAL:
		puts("vertex normal");
		for (i = 0; i < 3; ++i) {
			token = strsep(&string, " ");
			assert(token != NULL);
			v[i] = atof(token);
		}
		array_append(mesh->norms, &v);
		break;
	case OT_FACE:
		{
			puts("face");
			struct polygon p;
			p.verts = new_array(4, sizeof(struct vertex));

			while ((face = strsep(&string, " "))) {
				struct vertex v;

				token = strsep(&face, "/");
				v.pos_index = ((*token == '\0') ? 0 : atoi(token)) - 1;
				token = strsep(&face, "/");
				v.texco_index = ((*token == '\0') ? 0 : atoi(token)) - 1;
				token = strsep(&face, "/");
				v.norm_index = ((*token == '\0') ? 0 : atoi(token)) - 1;

				printf("vert: %d, texco:%d, norm: %d\n",
				       v.pos_index, v.texco_index, v.norm_index);

				array_append(p.verts, &v);
			}
			array_append(mesh->polys, &p);
		}
		break;
	default:
		puts("not parsed yet");
		break;
	}
	free(tofree);
	return 0;
}


int mesh_obj_read(struct accmesh *accmesh)
{
	struct filemap map;
	const char *buf;
	size_t buflen;
	char *linep = NULL;
	size_t linecap = 256;
	int linelen;
	struct generic_mesh *mesh;

	if (accmesh == NULL)
		return 1;

	mesh = accmesh->mesh;
	mesh_init(mesh);
	mapfile(&map, accmesh->filename, 0);

	buf = map.buf;
	buflen = map.size;
	linep = malloc(linecap);

	while ((linelen = amemgetline(&linep, &linecap, buf, buflen)) != -1) {
		if (!linelen)
			continue;
		parse_line(linep, linelen, mesh);
		buf += linelen;
		buflen -= linelen;
	}

	free(linep);
	unmapfile(&map, 0);
	return 0;
}
