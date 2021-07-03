#pragma once

#include "pch.h"

class Light
{
public:
	bool enabled = true;
	bool dontShowCorona = false;
	bool dontShowAreaLight = false;

	int id = -1;

	int r = 255;
	int g = 0;
	int b = 0;
	int a = 255;

	float radius = 0.0f;
	float farClip = 100.0f;
	float nearClip = 0.8f;

	float flashDistance = 40.0f;

	eCoronaType type = eCoronaType::CORONATYPE_SHINYSTAR;
	eCoronaFlareType flareType = eCoronaFlareType::FLARETYPE_NONE;

	CVector offset = CVector(0, 0, 0);
};