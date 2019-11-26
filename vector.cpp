#include "vector.h"

vector::vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };
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
