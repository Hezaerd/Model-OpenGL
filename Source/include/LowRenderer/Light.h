#pragma once
#include <Vec4/Vec4.h>
#include <Vec3/Vec3.h>

//base is directionnal
class Light
{
public:
	Light();
	Light(float d, float a, float s, lm::vec3 color, lm::vec3 dir);
	Light(float d, float a, float s);
	void setColor(lm::vec3 color);
	void setDir(lm::vec3 dir);
	void setPos(lm::vec3 pos);

	float diffuse;
	float ambient;
	float specular;
	lm::vec3 lightColor;
	lm::vec3 position;
	lm::vec3 direction;
};

//point light
class PointLight : public Light
{
public:
	PointLight(float d, float a, float s, float l, float exp, lm::vec3 color, lm::vec3 pos);
	PointLight(float d, float a, float s, float l, float exp);

	float linearAttenuation;
	float exponentialAttenuation;
};

class SpotLight : public Light
{
public:
	SpotLight(float d, float a, float s, float l, float exp, float cut, float outerCut, lm::vec3 color, lm::vec3 pos, lm::vec3 dir);
	SpotLight(float d, float a, float s, float l, float exp, float cut, float outerCut);

	float cutOff;
	float outerCutoff;
	float linearAttenuation;
	float exponentialAttenuation;
};
