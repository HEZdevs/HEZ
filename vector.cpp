#include "vector.h"
#include <cmath>

//constructor
vector::vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };

// ... functions
void vector::normalize()
{
	float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

// global functions
float dot(const vector& l, const vector& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}
vector cross(const vector& l, const vector& r)
{
	return vector(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
}
float length(const vector& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
float sqrlen(const vector& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
vector normalize(const vector& v)
{
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return vector(v.x / length, v.y / length, v.z / length);
}

// operators
vector operator +(const vector& l, const vector& r)
{
	return vector(l.x + r.x, l.y + r.y, l.z + r.z);
}
vector operator -(const vector& l, const vector& r)
{
	return vector(l.x - r.x, l.y - r.y, l.z - r.z);
}
vector operator *(const vector& l, const float& r)
{
	return vector(l.x * r, l.y * r, l.z * r);
}
vector operator *(const float& l, const vector& r)
{
	return vector(l * r.x, l * r.y, l * r.z);
}
vector operator /(const vector& l, const float& r)
{
	return vector(l.x / r, l.y / r, l.z / r);
}
