#pragma once
#include <SFML/Graphics.hpp>

class VectorMath {
public:
	static float dot(sf::Vector3f a, sf::Vector3f b);
	static sf::Vector3f corssProduct(const sf::Vector3f & a, const sf::Vector3f & b);
	static void normalize(sf::Vector3f & a);
	static float magnitudeSquared(const sf::Vector3f& a);
};