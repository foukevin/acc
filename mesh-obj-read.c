#include "mesh.h"
#include "mapfile.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h> /* memcpy */

/*
 * Reads a line from buf, delimited by the newline character.  The
 * delimiter character is included as part of the line, unless the
 * end of the memory chunk is reached.
 *
 * The caller may provide a pointer to a malloced buffer for the line
 * in *linep, and the capacity of that buffer in *linecapp.  This
 * function expand the buffer as needed, as if via realloc().  If linep
 * points to a NULL pointer, a new buffer will be allocated.
 *
 * In either case, *linep and *linecapp will be updated accordingly.
 *
 * The function returns the number of characters written, excluding the
 * terminating NUL character.  The value -1 is returned if an error
 * occurs, or if end-of-file is reached.
 */
ssize_t amemgetline(char **restrict linep, size_t *restrict linecapp,
		   const char *restrict buf, size_t buflen)
{
	size_t linelen;
	const char *c = buf;

	if ((buf == NULL) || (buflen <= 0))
		return -1;

	/* get line length */
	while (c < (buf + buflen) && (*c != '\n')) ++c;
	linelen = (c - buf) + 1;

	/* check linep and linecapp */
	if (*linep == NULL) {
		*linep = malloc(linelen + 1);
		*linecapp = linelen + 1;
	} else if (*linecapp < (linelen + 1)) {
		*linep = realloc(*linep, linelen + 1);
		*linecapp = linelen + 1;
	}

	/* neither malloc nor realloc worked */
	if (*linep == NULL)
		return -1;

	memcpy(*linep, buf, linelen);
	(*linep)[linelen] = '\0';

	return linelen;
}

ssize_t get_token(char *tokenp, size_t cap, const char *buf, size_t buflen)
{
	size_t tokenlen;
	const char *c = buf;

	if ((buf == NULL) || (buflen <= 0))
		return -1;

	if ((tokenp == NULL) || (cap <= 0))
		return -1;

	/* skip heading spaces */
	while (c < (buf + buflen) && (*c == ' ')) ++c;

	while (c < (buf + buflen) && (*c != ' ')) ++c;
	tokenlen = (c - buf);

	/* TODO: check cap before copy */
	memcpy(tokenp, buf, tokenlen);
	tokenp[tokenlen] = '\0';

	return tokenlen;
}

enum obj_token_type {
	OT_COMMENT,
	OT_OBJECT,
	OT_VERTEX,
	OT_VERTEX_NORMAL,
	OT_FACE,
	OT_UNKNOWN,
};

struct obj_token {
	const char *string;
	enum obj_token_type name;
} static const tokens[] = {
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
	double *v;
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
	int fverts = 0;

	switch (type) {
	case OT_COMMENT:
		puts("comment");
		break;
	case OT_VERTEX:
		puts("vertex");
		v = mesh_get_vertex(mesh);
		for (i = 0; i < 3; ++i) {
			token = strsep(&string, " ");
			assert(token != NULL);
			v[i] = atof(token);
		}
		break;
	case OT_VERTEX_NORMAL:
		puts("vertex normal");
		break;
	case OT_FACE:
		puts("face");
		fverts = 0;
		while ((face = strsep(&string, " "))) {
			token = strsep(&face, "/");
			int vert = (*token == '\0') ? -1 : atoi(token);
			token = strsep(&face, "/");
			int texco = (*token == '\0') ? -1 : atoi(token);
			token = strsep(&face, "/");
			int norm = (*token == '\0') ? -1 : atoi(token);
			printf("vert: %d, texco:%d, norm: %d\n", vert, texco, norm);
			fverts++;
		}

		switch (fverts) {
		case 4:

			/* fallthrought */
			puts("2nd tri");
		case 3:
			puts("1st tri");
			break;
		default:
			break;
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
