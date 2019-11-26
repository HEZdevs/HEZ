#ifndef VECTOR_H
#define VECTOR_H
struct vector 
{
	float x, y, z;
	vector(float x, float y, float z);
	void normalize();
};
vector operator +(const vector& l, const vector& r);
vector operator -(const vector& l, const vector& r);
vector operator *(const vector& l, const float& r);
vector operator *(const float& l, const vector& r);
vector operator /(const vector& l, const float& r);

float dot(const vector& l, const vector& r);
vector cross(const vector& l, const vector& r);
float length(const vector& v);
float sqrlen(const vector& v);
vector normalize(const vector& v);
#endif
