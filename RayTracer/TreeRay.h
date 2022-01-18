#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "ChunkTree.h"
#include "VectorMath.h"

/* RAY FÖR ATT SÖKA BLOCK I ETT OCTREE */
class TreeRay {
private:
	Octree* root;
	int search(Octree* parent, const sf::Vector3f relativePosition, int scale);
	int searchNonRecursive();
	//Variables:
	sf::Vector3f direction;
	sf::Vector3f startRelativePositon;
	int lastBlockFace = 0;
	float stepX, stepY, stepZ;
	float tDeltaX, tDeltaY, tDeltaZ;

public:
	//Search variables
	TreeRay(sf::Vector3f worldPosition, Octree* root);
	int shoot(sf::Vector3f thisItterationDirction);
	int reps = 0;
	inline int getFaceValue() {
		return lastBlockFace + 3 - (lastBlockFace > 0 ? 1 : 0);
	};
};