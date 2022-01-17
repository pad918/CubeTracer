#pragma once
#include <SFML/Graphics.hpp>
#include "WorldObject.h"
#include "Ray.h";

class Plane : public WorldObject
{
public:
	sf::Vector3f normal;
	virtual bool willHit(const Ray & ray); //Remove
	virtual float hitDistance(const Ray& ray);
	virtual sf::Vector3f& getNormal(const sf::Vector3f& pos);
	Plane();

private:

};