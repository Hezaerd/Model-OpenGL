#include "../../include/LowRenderer/Light.h"
#include <iostream>

Light::Light()
{
	diffuse = 0.5f;
	ambient = 0.5f;
	specular = 0.5f;
	lightColor = lm::vec3(1);
	direction = lm::vec3(0,0,-1);
	position = lm::vec3(0);
}

Light::Light(float d, float a, float s, lm::vec3 color, lm::vec3 dir)
{
	diffuse = d;
	ambient = a;
	specular = s;
	lightColor = color;
	direction = dir;
}

Light::Light(float d, float a, float s)
{
	diffuse = d;
	ambient = a;
	specular = s;
	lightColor = lm::vec3(1);
	direction = lm::vec3(lm::vec3::down);
}

void Light::setColor(lm::vec3 color)
{
	lightColor = color;
}

void Light::setDir(lm::vec3 dir)
{
	direction = dir;
}

void Light::setPos(lm::vec3 pos)
{
	position = pos;
}

PointLight::PointLight(float d, float a, float s, float l, float exp, lm::vec3 color, lm::vec3 pos)
{
	diffuse = d;
	ambient = a;
	specular = s;
	linearAttenuation = l;
	exponentialAttenuation = exp;
	lightColor = color;
	position = pos;
}

PointLight::PointLight(float d, float a, float s, float l, float exp)
{
	diffuse = d;
	ambient = a;
	specular = s;
	linearAttenuation = l;
	exponentialAttenuation = exp;
	lightColor = lm::vec3(1);
	position = lm::vec3(0);
}

SpotLight::SpotLight(float d, float a, float s, float l, float exp, float cut, float outerCut, lm::vec3 color, lm::vec3 pos, lm::vec3 dir)
{
	diffuse = d;
	ambient = a;
	specular = s;
	linearAttenuation = l;
	exponentialAttenuation = exp;
	lightColor = color;
	position = pos;
	direction = dir;
	cutOff = cut;
	outerCutoff = outerCut;
	if (cutOff <= outerCutoff) {
		std::cout << "cutOff smaller than outerCutOff !";
	}
}

SpotLight::SpotLight(float d, float a, float s, float l, float exp, float cut, float outerCut)
{
	diffuse = d;
	ambient = a;
	specular = s;
	linearAttenuation = l;
	exponentialAttenuation = exp;
	lightColor = lm::vec3(1);
	position = lm::vec3(0);
	direction = lm::vec3(lm::vec3::down);
	cutOff = cut;
	outerCutoff = outerCut;
	if (cutOff <= outerCutoff) {
		std::cout << "cutOff smaller than outerCutOff !";
	}
}
