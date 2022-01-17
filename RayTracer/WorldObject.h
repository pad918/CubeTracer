#pragma once
#include <SFML/Graphics.hpp>
#include "Ray.h"
#include "VectorMath.h"

class WorldObject {
protected:
	float glossiyNess = 1.0f;
	float color[3];

public:
	//virtual float hitDistance(Ray ray) = 0;
	virtual bool willHit(const Ray & ray) = 0; // To be removed
	virtual float hitDistance(const Ray & ray) = 0;
	virtual sf::Vector3f& getNormal(const sf::Vector3f & pos) = 0;
	sf::Vector3f position;
	sf::Color surfaceColor;
};