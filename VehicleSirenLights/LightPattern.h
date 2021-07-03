#pragma once

#include "pch.h"

enum LightPosition
{
	LEFT,
	RIGHT,
	MIDDLE
};

enum LightDirection
{
	ALL,
	FRONT_OR_BACK
};

struct LightPatternFrame
{
	int duration;
	bool leftLightState;
	bool rightLightState;
	bool middleLightState;
};

class LightPattern
{
private:
	std::vector<LightPatternFrame> m_Frames;
public:

	LightPatternFrame AddFrame(bool leftLightState, bool rightLightState, bool middleLightState, int durationMs);
	LightPatternFrame GetFrame(int index);
	int GetTotalFrames();
};

class LightPatterns
{
public:
	static std::vector<LightPattern*> m_Patterns;

	static LightPattern* NewPattern();
	static LightPattern* GetPattern(int index);
	static int GetTotalPatterns();
};