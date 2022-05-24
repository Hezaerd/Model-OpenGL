#include "../../include/LowRenderer/Camera.h"

#pragma region USING NAMESPACE
using namespace LowRenderer;
#pragma endregion

Camera::Camera(const float fov, const float width, const float height, const float near, const float far) :
	speed_walk(5.f), speed_sprint(50.f), first_mouse(true), lastX(800.0f / 2), lastY(600.0f / 2), yaw(0), pitch(0)
{
	const float aspect = width / height;

	speed = speed_walk;

	// Vec
	position = vec3(0.f, 0.f, 1.f);
	front = vec3(0.f, 0.f, 1.f);
	up = world_up = vec3(0.f, 1.f, 0.f);
	right = front.crossProduct(world_up).normalized();

	// Mat
	view_matrix.identity;
	//mat4 temp_view;
	//view_matrix = temp_view.look_at(position,position + front, up);
	projection_matrix = Mat4<float>::proj_perspective(fov, near, far);
}

void Camera::update_cam_vec()
{
    front.X() = cos(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));
    front.Y() = sin(degreesToRadians(pitch));
    front.Z() = sin(degreesToRadians(yaw)) * cos(degreesToRadians(pitch));

    front = front.normalized();
    right = front.crossProduct(world_up).normalized();
    up = right.crossProduct(front).normalized();
}

Mat4<float> Camera::update_view_matrix()
{
	mat4 temp_view;
    update_cam_vec();
    view_matrix = temp_view.look_at(position, position + front, up);

    return view_matrix;
}