#pragma once
#include "../../include/Ressources/Model.h"
#include "Mat4/Mat4.h"
#include "Mat3/Mat3.h"

#pragma region USING NAMESPACE
using namespace lm;
using namespace Ressource;
#pragma endregion

class Mesh
{
public:
	Mesh(Model* mod, const Vec3<float>& rotation, const Vec3<float>& position, const Vec3<float>& scale);

	Model* model;
	mat4 model_matrix;

	void update_uniform(Shader* shader, Camera& cam);
};
