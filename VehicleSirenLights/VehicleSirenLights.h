/*
	Required:
		DirectX 9 SDK
		Plugin SDK
		Visual Studio 2019 (v142)
		Windows SDK

	Disclaimer: I've done a total of 3 projects in C++ that actually works :v
*/

#pragma once

#include "pch.h"

class VehicleSirenLights : Hook
{
private:
	static void Draw();
	static void ProcessScripts();
	static void RenderVehicle(CVehicle* vehicle);
public:
	VehicleSirenLights();
};
