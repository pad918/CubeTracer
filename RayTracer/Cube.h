#pragma once
#include <SFML/Graphics.hpp>
#include "Ray.h"

class Cube {
public:
	unsigned int color = 0;
	sf::Vector3f boundsMin, boundsMax;
	Cube(sf::Vector3f min, sf::Vector3f max);
	Cube();
	bool doesIntersect(Ray & ray);
private:
	Cube *nodes;
	int layer = 0;
};