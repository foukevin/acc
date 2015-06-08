#include "acc-mesh.h"

#include <stdlib.h> /* exit */
#include <string.h> /* memset */

int main(int argc, char **argv)
{
	struct accmesh *accmesh, accmesh_storage;

	accmesh = &accmesh_storage;
	memset(accmesh, 0, sizeof(*accmesh));

	return accmesh->return_value;
}
