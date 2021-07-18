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

void DisableOriginalLights()
{
	//https://gtaforums.com/topic/757430-block-siren-lights-memory-address-for-it/

	//0A8C: write_memory 0x70026C size 4 value 0x90909090 virtual_protect 0
	plugin::patch::SetUInt(0x70026C, 0x90909090);

	//0A8C : write_memory 0x700270 size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x700270, 0x90);

	//0A8C : write_memory 0x700271 size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x700271, 0x90);

	//0A8C : write_memory 0x700261 size 4 value 0x90909090 virtual_protect 0
	plugin::patch::SetUInt(0x700261, 0x90909090);

	//0A8C : write_memory 0x700265 size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x700265, 0x90);

	//0A8C : write_memory 0x700266 size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x700266, 0x90);

	//0A8C : write_memory 0x700257 size 4 value 0x90909090 virtual_protect 0
	plugin::patch::SetUInt(0x700257, 0x90909090);

	//0A8C : write_memory 0x70025B size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x70025B, 0x90);

	//0A8C : write_memory 0x70025C size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x70025C, 0x90);

	//--

	//0@ = 0xC3F12C //CPointLight => RGB
	int pointLight = 0xC3F12C;

	//0A8C: write_memory 0@ size 4 value 0.0 virtual_protect 0 // R
	plugin::patch::SetUInt(pointLight, 0);

	//0@ += 4
	pointLight += 4;

	//0A8C: write_memory 0@ size 4 value 0.0 virtual_protect 0  // G
	plugin::patch::SetUInt(pointLight, 0);

	//0@ += 4
	pointLight += 4;

	//0A8C: write_memory 2@ size 4 value 0.0 virtual_protect 0 
	plugin::patch::SetUInt(pointLight, 0);

	//--

	//NOPs the function that draws the coronnas
	//0A8C: write_memory 0x6ABA60 size 4 value 0x90909090 virtual_protect 0
	plugin::patch::SetUInt(0x6ABA60, 0x90909090);

	//0A8C: write_memory 0x6ABA64 size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x6ABA64, 0x90);

	//--

	//NOPs the function that checks wether the siren was activated or not
	//0A8C: write_memory 0x6FFDFC size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x6FFDFC, 0x90);

	//0A8C: write_memory 0x6FFDFD size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x6FFDFD, 0x90);

	//0A8C: write_memory 0x6FFDFE size 1 value 0x90 virtual_protect 0
	plugin::patch::SetUChar(0x6FFDFE, 0x90);

	//--

	//NOPs the function that activates the shadow drawing under the vehicle
	//0A8C: write_memory 0x70802D size 4 value 0x90909090 virtual_protect 0
	//plugin::patch::SetUInt(0x70802D, 0x90909090);
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

	DisableOriginalLights();

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
