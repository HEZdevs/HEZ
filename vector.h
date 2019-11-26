#ifndef VECTOR_H
#define VECTOR_H
struct vector 
{
	float x, y, z;
	vector(float x, float y, float z);
};
vector operator +(const vector& l, const vector& r);
vector operator -(const vector& l, const vector& r);
vector operator *(const vector& l, const float& r);
vector operator *(const float& l, const vector& r);
vector operator /(const vector& l, const float& r);
#endif
