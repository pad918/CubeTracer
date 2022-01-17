#pragma once
#include <SFML/Graphics.hpp>

//Testad, FUNGERAR
struct Octree {
public:
	Octree**** nodes; // Fyrpelstack, är det dåligt?
	int* type;
};

class ChunkTree {
public:
	Octree* chunkRoot;
	ChunkTree();
	void instantiate(int*** cubes, int deapth);
	//void test();
	void fillQuadrant(Octree* octant); //Fungerar
	void rebuildTree();
	//Octree* getAtWorldPosition(sf::Vector3i target, sf::Vector3i current, Octree* parent, int deapth);
private:
	Octree* createOctree(int*** cubes, Octree* partent, int deapth, sf::Vector3i pos);
	int setVoid(Octree* partent, int deapth);
};


