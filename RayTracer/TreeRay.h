#pragma once
#include <SFML/Graphics.hpp>
#include "ChunkTree.h"
#include "VectorMath.h"

/* RAY FÖR ATT SÖKA BLOCK I ETT OCTREE */
class TreeRay {
private:
	Octree* root;
	int search(Octree* parent, sf::Vector3f relativePosition, int scale);
	//Variables:
	sf::Vector3f direction;
	int lastBlockFace = 0;
public:
	TreeRay(Octree * rootTree, sf::Vector3f direction);
	int shoot(sf::Vector3f relativePosition);
	int reps = 0;
	inline int getFaceValue() {
		return lastBlockFace + 3 - (lastBlockFace > 0 ? 1 : 0);
	};
};