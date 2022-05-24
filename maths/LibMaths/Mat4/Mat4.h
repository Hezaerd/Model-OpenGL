#pragma once

#include "../Vec4/Vec4.h"

namespace lm
{
	template <typename T> class Mat4
	{
	private:
		Vec4<T> matrix[4];

	public:
		static const Mat4<T> identity;

		// Constructors
		// ------------
		Mat4()
		{
			this->matrix[0] = Vec4<T>(1, 0, 0, 0);
			this->matrix[1] = Vec4<T>(0, 1, 0, 0);
			this->matrix[2] = Vec4<T>(0, 0, 1, 0);
			this->matrix[3] = Vec4<T>(0, 0, 0, 1);
		}

		Mat4(const T init)
		{
			this->matrix[0] = Vec4<T>(init, 0, 0, 0);
			this->matrix[1] = Vec4<T>(0, init, 0, 0);
			this->matrix[2] = Vec4<T>(0, 0, init, 0);
			this->matrix[2] = Vec4<T>(0, 0, 0, 1);
		}

		Mat4(const Vec4<T>& v1, const Vec4<T>& v2, const Vec4<T>& v3, const Vec4<T>& v4)
		{
			this->matrix[0] = v1;
			this->matrix[1] = v2;
			this->matrix[2] = v3;
			this->matrix[3] = v4;
		}

		Mat4(const Mat4<T>& mat4)
		{
			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = mat4.matrix[i];
		}

		Mat4(Mat4<T>&& mat4) noexcept
		{
			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = std::move(mat4.matrix[i]);
		}

		// Operator
		// --------
		Mat4<T>& operator=(const Mat4<T>& mat4)
		{
			if (this == &mat4)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = mat4.matrix[i];

			return *this;
		}

		Mat4<T>& operator=(Mat4<T>&& mat4) noexcept
		{
			if (this == &mat4)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->matrix[i] = std::move(mat4.matrix[i]);

			return *this;
		}

		const Vec4<T> operator[](int idx) const
		{
			if (idx >= 0 && idx < 4)
				return this->matrix[idx];
			return this->matrix[0];
		}

		Vec4<T>& operator[](int idx)
		{
			if (idx >= 0 && idx < 4)
				return this->matrix[idx];
			return this->matrix[0];
		}

		const T operator[](const char* idx) const
		{
			unsigned int vecIdx = idx[1] - '1';
			switch (idx[0])
			{
			case 'x': return this->matrix[vecIdx].X();
			case 'y': return this->matrix[vecIdx].Y();
			case 'z': return this->matrix[vecIdx].Z();
			case 'w': return this->matrix[vecIdx].W();

			default: return this->matrix[0].X();
			}
		}

		T& operator[](const char* idx)
		{
			unsigned int vecIdx = idx[1] - '0';
			switch (idx[0])
			{
			case 'x': return this->matrix[vecIdx].X();
			case 'y': return this->matrix[vecIdx].Y();
			case 'z': return this->matrix[vecIdx].Z();
			case 'w': return this->matrix[vecIdx].W();

			default: return this->matrix[0].X();
			}
		}

		Mat4<T> operator*(const Mat4<T>& mat4) const
		{
			Mat4<T> newMat4;

			for (unsigned int i = 0; i < 4; i++)
			{
				Vec4<T> vec4;
				for (unsigned int j = 0; j < 4; j++)
				{
					vec4[j] = this->matrix[0][j] * mat4.matrix[i].X()
						+ this->matrix[1][j] * mat4.matrix[i].Y()
						+ this->matrix[2][j] * mat4.matrix[i].Z()
						+ this->matrix[3][j] * mat4.matrix[i].W();
				}
				newMat4.matrix[i] = vec4;
			}
			
			return newMat4;
		}
		
		Vec4<T> operator*(const Vec4<T>& vec4) const
		{
			Vec4<T> new_vec4;
			new_vec4.X() = (this->matrix[0][0] * vec4.X()) + (this->matrix[1].X() * vec4.Y()) + (this->matrix[2].X() * vec4.Z()) + (this->matrix[3].X() * vec4.W());
			new_vec4.Y() = (this->matrix[0][1] * vec4.X()) + (this->matrix[1].Y() * vec4.Y()) + (this->matrix[2].Y() * vec4.Z()) + (this->matrix[3].Y() * vec4.W());
			new_vec4.Z() = (this->matrix[0].Z() * vec4.X()) + (this->matrix[1].Z() * vec4.Y()) + (this->matrix[2].Z() * vec4.Z()) + (this->matrix[3].Z() * vec4.W());
			float x = this->matrix[0].W() * vec4.X();
			float y = this->matrix[1].W() * vec4.Y();
			float z = this->matrix[2].W() * vec4.Z();
			float w = this->matrix[3].W() * vec4.W();
			float result = x + y + z + w;
			new_vec4.W() = (this->matrix[0].W() * vec4.X()) + (this->matrix[1].W() * vec4.Y()) + (this->matrix[2].W() * vec4.Z()) + (this->matrix[3].W() * vec4.W());
			return new_vec4;
		}

		const bool operator==(const Mat4<T>& mat4) const
		{
			if (this == &mat4)
				return true;

			for (unsigned int i = 0; i < 4; i++)
				if (this->matrix[i] != mat4.matrix[i])
					return false;

			return true;
		}

		const bool operator!=(const Mat4<T>& mat4) const
		{
			return !(*this == mat4);
		}

		// Get methodes
		// ------------
		Mat4<T> get_inverse()
		{
			// Rotation in Y
			this->matrix[2].X() *= -1;
			this->matrix[0].Z() *= -1;

			// Rotation in Y
			this->matrix[2].Y() *= -1;
			this->matrix[1].Z() *= -1;

			// Tranlation
			this->matrix[3] *= -1;
			return *this;
		}

		// Transformations
		// ---------------
		static Mat4<T> translation(const Vec3<T>& translation)
		{
			Mat4<T> translate;
			translate["x3"] = translation.X();
			translate["y3"] = translation.Y();
			translate["z3"] = translation.Z();
			return translate;
		}

		static Mat4<T> scale(const Vec3<T>& scale)
		{
			Mat4<T> matrix_scale;
			matrix_scale["x0"] = scale.X();
			matrix_scale["y1"] = scale.Y();
			matrix_scale["z2"] = scale.Z();
			return matrix_scale;
		}

		static Mat4<T> x_rotation(const float angle)
		{
			const float rad = static_cast<float>(Vec4<T>::degreesToRadians(static_cast<double>(angle)));

			Mat4<T> matrix_scale;
			matrix_scale["y1"] = std::cosf(rad);
			matrix_scale["y2"] = -std::sinf(rad);

			matrix_scale["z1"] = std::sinf(rad);
			matrix_scale["z2"] = std::cosf(rad);
			
			return matrix_scale;
		}

		static Mat4<T> y_rotation(const float angle)
		{
			const float rad = static_cast<float>(Vec4<T>::degreesToRadians(static_cast<double>(angle)));

			Mat4<T> matrix_scale;
			matrix_scale["x0"] = std::cosf(rad);
			matrix_scale["x2"] = std::sinf(rad);

			matrix_scale["z0"] = -std::sinf(rad);
			matrix_scale["z2"] = std::cosf(rad);

			return matrix_scale;
		}

		static Mat4<T> z_rotation(const float angle)
		{
			const float rad = static_cast<float>(Vec4<T>::degreesToRadians(static_cast<double>(angle)));

			Mat4<T> matrix_scale;
			matrix_scale["x0"] = std::cosf(rad);
			matrix_scale["x1"] = -std::sinf(rad);

			matrix_scale["y0"] = std::sinf(rad);
			matrix_scale["y1"] = std::cosf(rad);

			return matrix_scale;
		}

		static Mat4<T> create_transformation(const Vec3<T>& rotation, const Vec3<T>& position, const Vec3<T>& scaleVec)
        {
            return translation(position) * z_rotation(rotation.Z()) * x_rotation(rotation.X()) * y_rotation(rotation.Y()) * scale(scaleVec);
        }

		// Opengl Maths
		// ------------
		static Mat4<T> proj_perspective(const float &fov, const float &near, const float &far)
		{
			Mat4<T> proj_matrix;

			float scale = 1 / tan(fov * 0.5 * M_PI / 180); 
		    proj_matrix[0][0] = scale;
		    proj_matrix[1][1] = scale;
		    proj_matrix[2][2] = -far / (far - near);
		    proj_matrix[3][2] = -far * near / (far - near);
		    proj_matrix[2][3] = -1;
		    proj_matrix[3][3] = 0;

			return proj_matrix;
		}

		Mat4 look_at(const vec3& from, const vec3& to, const vec3& vector_up = vec3(0, 1, 0))
        {
            vec3 forward = (to - from).normalized();
            vec3 right = forward.crossProduct(vector_up).normalized();
            vec3 up = right.crossProduct(forward);

            Mat4 view_matrix;

            view_matrix[0][0] = right.X();
            view_matrix[1][0] = right.Y();
            view_matrix[2][0] = right.Z();

            view_matrix[0][1] = up.X();
            view_matrix[1][1] = up.Y();
            view_matrix[2][1] = up.Z();

            view_matrix[0][2] = -forward.X();
            view_matrix[1][2] = -forward.Y();
            view_matrix[2][2] = -forward.Z();

            view_matrix[3][0] = -right.dotProduct(from);
            view_matrix[3][1] = -up.dotProduct(from);
            view_matrix[3][2] = forward.dotProduct(from);

            return view_matrix;
        }
	};

	template<class T> const Mat4<T> Mat4<T>::identity = Mat4<T>();

	typedef Mat4<float> mat4;
}