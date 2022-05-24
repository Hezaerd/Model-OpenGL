#pragma once
#include "Mat4/Mat4.h"
#include "Vec2/Vec2.h"
#include "Vec3/Vec3.h"

#pragma region USING NAMESPACE
using namespace lm;
#pragma endregion

namespace LowRenderer
{
	class Camera
	{
	public:
		float speed;
		float speed_walk;
		float speed_sprint;

		vec3 position;
		vec3 front;
		vec3 up;
		vec3 right;
		vec3 world_up;

		mat4 view_matrix;
		mat4 projection_matrix;

		bool first_mouse;

		float lastX;
		float lastY;

		float yaw;
		float pitch;

		Camera(float fov, float width, float height, float near, float far);

		void update_cam_vec();

		Mat4<float> update_view_matrix();
	};
}