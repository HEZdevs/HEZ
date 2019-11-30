#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include "vector.h"

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
		///
		inline vector transform(const vector& v) {
			return vector(v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0],
                          v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1],
                          v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2]);
		}
		/// статические функции
		// возвращает единичную матрицу
		static inline matrix getIdentity() {
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
		//(инвертированная, я полагаю)
		static inline matrix createViewLookAt(const vector& cameraPosition, const vector& cameraTarget, const vector& cameraUp) {
			vector f = vector::normalize(cameraPosition - cameraTarget);
            vector s = vector::normalize(vector::cross(f, cameraUp));
            vector v = vector::cross(s, f);
            matrix m;
            m.m[0][0] = s.x;
            m.m[0][1] = s.y;
            m.m[0][2] = s.z;
            m.m[0][3] = -vector::dot(s, cameraPosition);
            m.m[1][0] = v.x;
            m.m[1][1] = v.y;
            m.m[1][2] = v.z;
            m.m[1][3] = -vector::dot(v, cameraPosition);
            m.m[2][0] = f.x;
            m.m[2][1] = f.y;
            m.m[2][2] = f.z;
            m.m[2][3] = -vector::dot(f, cameraPosition);
            m.m[3][3] = 1.0f;
            return m;
		}
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
