#include "pch.h"

#include "VehicleSirenLights.h"

#include "LightPattern.h"
#include "SirenData.h"
#include "VehicleInfo.h"
#include "Menu.h"

void VehicleSirenLights::RenderVehicle(CVehicle* vehicle)
{
	if (!VehicleInfo::HasVehicle(vehicle)) return;

	VehicleInfo* vehicleInfo = VehicleInfo::GetVehicle(vehicle);

	vehicleInfo->m_nRenderTime = CTimer::m_snTimeInMilliseconds;
}

void VehicleSirenLights::ProcessScripts()
{
	Menu::Update();
	VehicleInfo::ProccessVehicles();

}

void VehicleSirenLights::Draw()
{
	Menu::Draw();
}

void WaitForInitEvent()
{
	static bool initialized = false;

	Events::initGameEvent += []
	{
		initialized = true;
	};

	while (!initialized) Sleep(100);
}

void SetupPattern()
{
	LightPattern* pattern1 = LightPatterns::NewPattern();
	pattern1->AddFrame(1, 0, 1, 160);
	pattern1->AddFrame(0, 0, 0, 50);
	pattern1->AddFrame(0, 1, 1, 160);
	pattern1->AddFrame(0, 0, 0, 50);

	LightPattern* pattern2 = LightPatterns::NewPattern();
	pattern2->AddFrame(1, 1, 1, 60);
	pattern2->AddFrame(0, 0, 0, 60);
	pattern2->AddFrame(1, 1, 1, 60);
	pattern2->AddFrame(0, 0, 0, 300);

	LightPattern* pattern3 = LightPatterns::NewPattern();
	pattern3->AddFrame(1, 0, 1, 60);
	pattern3->AddFrame(0, 0, 0, 60);
	pattern3->AddFrame(1, 0, 1, 60);

	pattern3->AddFrame(0, 0, 0, 300);

	pattern3->AddFrame(0, 1, 1, 60);
	pattern3->AddFrame(0, 0, 0, 60);
	pattern3->AddFrame(0, 1, 1, 60);

	pattern3->AddFrame(0, 0, 0, 300);

	LightPattern* pattern4 = LightPatterns::NewPattern();
	pattern4->AddFrame(1, 0, 1, 80);
	pattern4->AddFrame(0, 0, 0, 60);
	pattern4->AddFrame(1, 0, 1, 80);
	pattern4->AddFrame(0, 0, 0, 60);
	pattern4->AddFrame(1, 0, 1, 80);

	pattern4->AddFrame(0, 0, 0, 300);

	pattern4->AddFrame(0, 1, 1, 80);
	pattern4->AddFrame(0, 0, 0, 60);
	pattern4->AddFrame(0, 1, 1, 80);
	pattern4->AddFrame(0, 0, 0, 60);
	pattern4->AddFrame(0, 1, 1, 80);



}

void Initialize(void* param)
{
	WaitForInitEvent();

	//flog << "Init" << std::endl;

	if (GetModuleHandle("SAMP.dll"))
	{
		Globals::m_bIsSamp = true;
	}

	SetupPattern();

	SirenData::LoadSirens();

	auto plugin = new VehicleSirenLights;
}

VehicleSirenLights::VehicleSirenLights()
{
	window_callback = std::bind(&Draw);

	Events::processScriptsEvent += ProcessScripts;
	Events::vehicleRenderEvent += RenderVehicle;

}



BOOL WINAPI DllMain(HINSTANCE hDllHandle, DWORD nReason, LPVOID Reserved)
{
	if (nReason == DLL_PROCESS_ATTACH)
	{
		uint gameVersion = GetGameVersion();
		if (gameVersion == GAME_10US_HOODLUM || gameVersion == GAME_10US_COMPACT)
			CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&Initialize, nullptr, NULL, nullptr);
		else
			MessageBox(HWND_DESKTOP, "GTA SA v1.0 US is required", "Error", MB_ICONERROR);
	}

	return TRUE;
}
