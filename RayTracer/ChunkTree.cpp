#include "ChunkTree.h"
#include <iostream>

ChunkTree::ChunkTree()
{
	
}

void ChunkTree::instantiate(int*** cubes, int deapth)
{
	chunkRoot = createOctree(cubes, chunkRoot, deapth, sf::Vector3i(15, 15, 15));
	*chunkRoot->type = 1;
	//setVoid(chunkRoot, 5);
}
/*
void ChunkTree::test()
{
	Octree* n = chunkRoot;
	while (n->nodes != nullptr) {
		int r = rand() % 8;
		n = n->nodes[r];
	}
	*n->type = 6;// +rand() % 8;
}
*/
/* INTE KLAR *//*
Octree* ChunkTree::getAtWorldPosition(sf::Vector3i target, sf::Vector3i current, Octree* parent, int deapth)
{
	int x = current.x < target.x ? 1 : -1, y = current.y < target.y ? 1 : -1, z = current.z < target.z ? 1 : -1;
	int i = (z > 0 ? 4 : 0) + ((y > 0) ? 2 : 0) + ((x > 0) ? 1 : 0);

	if (deapth == 1) {
		return parent->nodes[3];
	}
	int pow = (2 << (deapth - 3)); if (pow == 0) { pow = 1; }
	return getAtWorldPosition(target, current + sf::Vector3i(x, y, z) * pow, parent->nodes[i], deapth - 1);
}
*/
void ChunkTree::fillQuadrant(Octree* octant)
{
	if (octant->nodes != nullptr) {
		for (int i = 0; i < 8; i++) {
			fillQuadrant(octant->nodes[i%2][(i/2)%2][i/4]);
		}
	}
	else {
		*octant->type = 1 + rand() % 4;
	}
}

void ChunkTree::rebuildTree()
{
	setVoid(chunkRoot, 5);
}

Octree* ChunkTree::createOctree(int*** cubes, Octree* partent, int deapth, sf::Vector3i pos)
{
	Octree* node = new Octree();
	int delta = 2 << (deapth - 3); // Inte helt säker
	if (deapth - 3 < 0)
		delta = 1;

	if (deapth > 1) {
		node->type = new int(1); // att ändra... (söka upp ur trädet?)
		node->nodes = new Octree *** [2]; 

		for (int x = 0; x < 2; x++) {
			node->nodes[x] = new Octree * *[2];
			for (int y = 0; y < 2; y++)
				node->nodes[x][y] = new Octree * [2];
		}

		for (int i = 0; i < 8; i++) {
			int z = -1 + 2 * (i / 4);
			int y = -1 + 2 * ((i / 2) % 2); // Tror att det fungerar
			int x = -1 + 2 * (i % 2);
			node->nodes[i%2][(i/2)%2][i/4] = createOctree(cubes, node, deapth - 1, pos + delta * sf::Vector3i(x, y, z));
		}
	}
	else if (deapth == 1) {
		node->type = new int(1); // att ändra... (söka upp ur trädet?)
		node->nodes = new Octree*** [2];
		for (int x = 0; x < 2; x++) {
			node->nodes[x] = new Octree** [2];
			for (int y = 0; y < 2; y++)
				node->nodes[x][y] = new Octree* [2];
		}

		for (int i = 0; i < 8; i++) {
			int z = (i / 4);
			int y = ((i / 2) % 2); // Tror att det fungerar
			int x = (i % 2);
			node->nodes[i % 2][(i / 2) % 2][i / 4] = createOctree(cubes, node, deapth - 1, pos + delta * sf::Vector3i(x, y, z));
		}
	}
	else {
		node->type = &cubes[pos.x][pos.y][pos.z];
		node->nodes = nullptr;
	}
	

	return node;
}

int ChunkTree::setVoid(Octree* partent, int deapth)
{
	if (deapth == 0) {
		return (*partent->type)!=0 ? 1 : 0;
	}
	int val = 0;
	for (int i = 0; i < 8; i++) {
		*partent->type = setVoid(partent->nodes[i % 2][(i / 2) % 2][i / 4], deapth - 1);
		if (*partent->type != 0)
			val = 1;
		//Uppdaterar kanske inte alla om detta är med:
		//if (*partent->type != 0)
		//	break;
	}
	*partent->type = val;
	return *partent->type;
}
