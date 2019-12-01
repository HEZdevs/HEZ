#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include "vector.h"
using namespace std;

namespace hez {
	struct matrix {
		float m[4][4];
		// конструктор, инициализирует матрицу значениями
		inline matrix(float m11, float m12, float m13, float m14, 
					  float m21, float m22, float m23, float m24, 
					  float m31, float m32, float m33, float m34, 
					  float m41, float m42, float m43, float m44) {
			m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
			m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
			m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
			m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;
		}
		// конструктор, инициализирует матрицу нулями
		inline matrix() {
			m[0][0] = 0.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 0.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 0.0f; m[2][3] = 0.0f;
			m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 0.0f;
		}
		/// возвращает вектор, умноженный на матрицу
		inline vector transform(const vector& v)
        {
            float w = 1 / ((((v.x * m[0][3]) + (v.y * m[1][3])) + (v.z * m[2][3])) + m[3][3]);
            return vector(((v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0]) + m[3][0]) * w, ((v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1]) + m[3][1]) * w, ((v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2]) + m[3][2]) * w);
        } 
		/// статические функции
		// возвращает единичную матрицу
		static inline matrix createIdentity() {
			matrix m;
			m.m[0][0] = 1.0f; m.m[0][1] = 0.0f; m.m[0][2] = 0.0f; m.m[0][3] = 0.0f;
			m.m[1][0] = 0.0f; m.m[1][1] = 1.0f; m.m[1][2] = 0.0f; m.m[1][3] = 0.0f;
			m.m[2][0] = 0.0f; m.m[2][1] = 0.0f; m.m[2][2] = 1.0f; m.m[2][3] = 0.0f;
			m.m[3][0] = 0.0f; m.m[3][1] = 0.0f; m.m[3][2] = 0.0f; m.m[3][3] = 1.0f;
			return m;
		}
		static inline matrix createProjectionOrthographic(float screenWidth, float screenHeight, float nearPlane, float farPlane) {
			matrix m;
            m.m[0][0] = 2.0f / screenWidth;
            m.m[1][1] = 2.0f / screenHeight;
            m.m[2][2] = 1.0f / (nearPlane - farPlane);
            m.m[3][2] = nearPlane / (nearPlane - farPlane);
            m.m[3][3] = 1.0f;
            return m;
		}
		// возвращает матрицу проекции
		static inline matrix createProjectionPerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
			matrix m;
            float a = 1.0f / tan(fieldOfView * 0.5f);
            m.m[0][0] = a / aspectRatio;
            m.m[1][1] = a;
            m.m[2][2] = farPlane / (nearPlane - farPlane);
            m.m[2][3] = -1.0f;
            m.m[3][2] = (nearPlane * farPlane) / (nearPlane - farPlane);
            return m;
		}
		static inline matrix createLookAt(vector pos, vector target, vector up)
        {
            vector nForward = vector::normalize(pos - target);
            vector nRight = vector::normalize(vector::cross(up, nForward));
            vector nUp = vector::cross(nForward, nRight);
            matrix m;
            m.m[0][0] = nRight.x;
            m.m[0][1] = nUp.x;
            m.m[0][2] = nForward.x;
            m.m[1][0]= nRight.y;
            m.m[1][1] = nUp.y;
            m.m[1][2] = nForward.y;
            m.m[2][0] = nRight.z;
            m.m[2][1] = nUp.z;
            m.m[2][2] = nForward.z;
            m.m[3][0] = -vector::dot(nRight, pos);
            m.m[3][1] = -vector::dot(nUp, pos);
            m.m[3][2] = -vector::dot(nForward, pos);
            m.m[3][3] = 1.0f;
            return m;
        }                   
		static inline matrix createLookAtInverted(vector pos, vector target, vector up) {
            vector nForward = vector::normalize(pos - target);
            vector nRight = vector::normalize(vector::cross(up, nForward));
            vector nUp = vector::cross(nForward, nRight);
            matrix m;
            m.m[0][0] = nRight.x;
            m.m[0][1] = nRight.y;
            m.m[0][2] = nRight.z;
            m.m[1][0] = nUp.x;
            m.m[1][1] = nUp.y;
            m.m[1][2] = nUp.z;
            m.m[2][0] = nForward.x;
            m.m[2][1] = nForward.y;
            m.m[2][2] = nForward.z;
            m.m[3][0] = pos.x;
            m.m[3][1] = pos.y;
            m.m[3][2] = pos.z;
            m.m[3][3] = 1.0f;
            return m;
        }
		static inline matrix createTranslation(const vector& v) {
			matrix m;
			m.m[0][0] = 1.0f;
            m.m[1][1] = 1.0f;
            m.m[2][2] = 1.0f;
            m.m[3][3] = 1.0f;
            m.m[3][0] = v.x;
            m.m[3][1] = v.y;
            m.m[3][2] = v.z;
            return m;
		}
		static inline matrix createRotationX(float ang)
        {
            float cosA = (float)cos(ang);
            float sinA = (float)sin(ang);
			matrix m;
            m.m[0][0] = 1.0f;
            m.m[1][1] = cosA;
            m.m[1][2] = sinA;
            m.m[2][1] = -sinA;
            m.m[2][2] = cosA;
            m.m[3][3] = 1.0f;
            return m;
        }
        static inline matrix createRotationY(float ang)
        {
            float cosA = (float)cos(ang);
            float sinA = (float)sin(ang);
			matrix m;
            m.m[0][0] = cosA;
            m.m[0][2] = sinA;
            m.m[2][0] = -sinA;
            m.m[1][1] = 1.0f;
            m.m[2][2] = cosA;
            m.m[3][3] = 1.0f;
            return m;
        }
        static inline matrix createRotationZ(float ang)
        {
            float cosA = (float)cos(ang);
            float sinA = (float)sin(ang);
            matrix m;
            m.m[0][0] = cosA;
            m.m[0][1] = sinA;
            m.m[1][0] = -sinA;
            m.m[1][1] = cosA;
            m.m[2][2] = 1.0f;
            m.m[3][3] = 1.0f;
            return m;
        }
        //static inline matrix createRotation(float x, float y, float z) { return createRotationX(x) * createRotationY(y) * createRotationZ(z); }
	};
	
	/// перегрузка операторов
	// умножение матриц
	inline matrix operator *(const matrix& l, const matrix& r) {
		matrix m;
		m.m[0][0] = l.m[0][0] * r.m[0][0] + l.m[0][1] * r.m[1][0] + l.m[0][2] * r.m[2][0] + l.m[0][3] * r.m[3][0];
		m.m[0][1] = l.m[0][0] * r.m[0][1] + l.m[0][1] * r.m[1][1] + l.m[0][2] * r.m[2][1] + l.m[0][3] * r.m[3][1];
		m.m[0][2] = l.m[0][0] * r.m[0][2] + l.m[0][1] * r.m[1][2] + l.m[0][2] * r.m[2][2] + l.m[0][3] * r.m[3][2];
		m.m[0][3] = l.m[0][0] * r.m[0][3] + l.m[0][1] * r.m[1][3] + l.m[0][2] * r.m[2][3] + l.m[0][3] * r.m[3][3];
		m.m[1][0] = l.m[1][0] * r.m[0][0] + l.m[1][1] * r.m[1][0] + l.m[1][2] * r.m[2][0] + l.m[1][3] * r.m[3][0];
		m.m[1][1] = l.m[1][0] * r.m[0][1] + l.m[1][1] * r.m[1][1] + l.m[1][2] * r.m[2][1] + l.m[1][3] * r.m[3][1];
		m.m[1][2] = l.m[1][0] * r.m[0][2] + l.m[1][1] * r.m[1][2] + l.m[1][2] * r.m[2][2] + l.m[1][3] * r.m[3][2];
		m.m[1][3] = l.m[1][0] * r.m[0][3] + l.m[1][1] * r.m[1][3] + l.m[1][2] * r.m[2][3] + l.m[1][3] * r.m[3][3];
		m.m[2][0] = l.m[2][0] * r.m[0][0] + l.m[2][1] * r.m[1][0] + l.m[2][2] * r.m[2][0] + l.m[2][3] * r.m[3][0];
		m.m[2][1] = l.m[2][0] * r.m[0][1] + l.m[2][1] * r.m[1][1] + l.m[2][2] * r.m[2][1] + l.m[2][3] * r.m[3][1];
		m.m[2][2] = l.m[2][0] * r.m[0][2] + l.m[2][1] * r.m[1][2] + l.m[2][2] * r.m[2][2] + l.m[2][3] * r.m[3][2];
		m.m[2][3] = l.m[2][0] * r.m[0][3] + l.m[2][1] * r.m[1][3] + l.m[2][2] * r.m[2][3] + l.m[2][3] * r.m[3][3];
		m.m[3][0] = l.m[3][0] * r.m[0][0] + l.m[3][1] * r.m[1][0] + l.m[3][2] * r.m[2][0] + l.m[3][3] * r.m[3][0];
		m.m[3][1] = l.m[3][0] * r.m[0][1] + l.m[3][1] * r.m[1][1] + l.m[3][2] * r.m[2][1] + l.m[3][3] * r.m[3][1];
		m.m[3][2] = l.m[3][0] * r.m[0][2] + l.m[3][1] * r.m[1][2] + l.m[3][2] * r.m[2][2] + l.m[3][3] * r.m[3][2];
		m.m[3][3] = l.m[3][0] * r.m[0][3] + l.m[3][1] * r.m[1][3] + l.m[3][2] * r.m[2][3] + l.m[3][3] * r.m[3][3];
		return m;
	}
}

#endif
