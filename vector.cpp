#include "vector.h"
#include <cmath>

//constructor
vector::vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };

// ... functions
/// нормализует вектор (оставляя то же направление, делает длину равную 1)
void vector::normalize()
{
	float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	this->x /= length;
	this->y /= length;
	this->z /= length;
}

// глобальные функции
/// скалярное произведение
float dot(const vector& l, const vector& r)
{
	return l.x * r.x + l.y * r.y + l.z * r.z;
}
/// векторное произведение
vector cross(const vector& l, const vector& r)
{
	return vector(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
}
/// возвращает длину вектора
float length(const vector& v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
/// возвращает квадрат длины вектора 
/// (быстрее, т.к. нету sqrt, иногда таким образом можно оптимизировать (например сравнение с константой))
float sqrlen(const vector& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
/// возвращает нормализованный вектор
vector normalize(const vector& v)
{
	float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return vector(v.x / length, v.y / length, v.z / length);
}

// operators
/// сложение векторов
vector operator +(const vector& l, const vector& r)
{
	return vector(l.x + r.x, l.y + r.y, l.z + r.z);
}
/// вычитание векторов
vector operator -(const vector& l, const vector& r)
{
	return vector(l.x - r.x, l.y - r.y, l.z - r.z);
}
/// умножение вектора на скаляр (V * f)
vector operator *(const vector& l, const float& r)
{
	return vector(l.x * r, l.y * r, l.z * r);
}
/// умножение вектора на скаляр (f * V)
vector operator *(const float& l, const vector& r)
{
	return vector(l * r.x, l * r.y, l * r.z);
}
/// деление вектора на скаляр (V / f)
vector operator /(const vector& l, const float& r)
{
	return vector(l.x / r, l.y / r, l.z / r);
}
