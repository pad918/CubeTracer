#pragma once
#include <SFML/Graphics.hpp>
#include "WorldObject.h"

class Sphere : public WorldObject {
public:
	Sphere(float r);
	virtual bool willHit(const Ray& ray);
	virtual float hitDistance(const Ray& ray);
	virtual sf::Vector3f& getNormal(const sf::Vector3f& pos);

private:
	float radius = 20.0f;
};