#include "Cube.h"
#define swap(a,b)       \
{                       \
    float TEMP = a;     \
    a = b;              \
    b = TEMP;           \
}                       \

Cube::Cube()
{

}

Cube::Cube(sf::Vector3f min, sf::Vector3f max)
{
	this->boundsMax = max;
	this->boundsMin = min;
}



bool Cube::doesIntersect(Ray& ray)
{
    /*
    float tmin = (boundsMin.x - ray.position.x) / ray.direction.x;
    float tmax = (boundsMax.x - ray.position.x) / ray.direction.x;

    if (tmin > tmax) { swap(tmin, tmax) };

    float tymin = (boundsMin.y - ray.position.y) / ray.direction.y;
    float tymax = (boundsMax.y - ray.position.y) / ray.direction.y;

    if (tymin > tymax) {swap(tymin, tymax)};

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (boundsMin.z - ray.position.z) / ray.direction.z;
    float tzmax = (boundsMax.z - ray.position.z) / ray.direction.z;

    if (tzmin > tzmax) { swap(tzmin, tzmax) };

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    if (tzmin > tmin)
        tmin = tzmin;

    if (tzmax < tmax)
        tmax = tzmax;
    */
    return true;
}
