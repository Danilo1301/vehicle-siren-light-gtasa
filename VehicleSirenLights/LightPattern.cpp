#include "LightPattern.h"

std::vector<LightPattern*> LightPatterns::m_Patterns;

LightPattern* LightPatterns::NewPattern()
{
	LightPattern* pattern = new LightPattern();

	m_Patterns.push_back(pattern);

	return pattern;
}

LightPattern* LightPatterns::GetPattern(int index)
{
	return m_Patterns.at(index);
}

int LightPatterns::GetTotalPatterns()
{
	return m_Patterns.size();
}







LightPatternFrame LightPattern::AddFrame(bool leftLightState, bool rightLightState, bool middleLightState, int durationMs)
{
	LightPatternFrame frame;

	frame.duration = durationMs;
	frame.leftLightState = leftLightState;
	frame.rightLightState = rightLightState;
	frame.middleLightState = middleLightState;

	m_Frames.push_back(frame);

	return frame;
}

LightPatternFrame LightPattern::GetFrame(int index)
{
	return m_Frames.at(index);
}

int LightPattern::GetTotalFrames()
{
	return m_Frames.size();
}