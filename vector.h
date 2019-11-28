#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace hez {
	struct vector {
		float x, y, z;
		// �����������
		inline vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { };
		/// ? �������
		// ����������� ������
		inline void normalize() {
			float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
			this->x /= length;
			this->y /= length;
			this->z /= length;
		}
		/// ����������� �������
		// ��������� ������������ ��������
		static inline float dot(const vector& l, const vector& r) {
			return l.x * r.x + l.y * r.y + l.z * r.z;
		}
		// ��������� ������������ 
		static inline vector cross(const vector& l, const vector& r) {
			return vector(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
		}
		// ���������� ����� �������
		static inline float length(const vector& v) {
			return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
		}
		// ���������� ������� ����� �������
		static inline float sqrlen(const vector& v) {
			return v.x * v.x + v.y * v.y + v.z * v.z;
		}
		// ���������� ��������������� ������ (����� = 1)
		static inline vector normalize(const vector& v) {
			float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
			return vector(v.x / length, v.y / length, v.z / length);
		}
		// �������� ������������
		static inline vector lerp(const vector& a, const vector& b, float t) {
			return vector(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
		}
	};
	/// ���������� ����������
	// �������� ��������
	inline vector operator +(const vector& l, const vector& r) {
		return vector(l.x + r.x, l.y + r.y, l.z + r.z);
	}
	// ��������� ��������
	inline vector operator -(const vector& l, const vector& r) {
		return vector(l.x - r.x, l.y - r.y, l.z - r.z);
	}
	// ��������� ������� �� ������ (������ ������, v * f)
	inline vector operator *(const vector& l, const float& r) {
		return vector(l.x * r, l.y * r, l.z * r);
	}
	// ��������� ������� �� ������ (������ �����, f * v)
	inline vector operator *(const float& l, const vector& r) {
		return vector(l * r.x, l * r.y, l * r.z);
	}
	// ������� ������� �� ������
	inline vector operator /(const vector& l, const float& r) {
		return vector(l.x / r, l.y / r, l.z / r);
	}
	// �������������� �������
	inline vector operator -(const vector& r) {
		return vector(-r.x, -r.y, -r.z);
	}
}
#endif
