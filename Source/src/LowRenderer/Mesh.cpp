#include "../../include/LowRenderer/Mesh.h"

Mesh::Mesh(Model* mod, const Vec3<float>& rotation, const Vec3<float>& position, const Vec3<float>& scale)
	: model(mod)
{
	model_matrix = lm::Mat4<float>::create_transformation(rotation, position, scale);
}
