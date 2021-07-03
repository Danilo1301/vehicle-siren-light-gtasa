#pragma once

#include "pch.h"
#include "Light.h"

class VehicleInfo
{
public:
	static std::map<int, VehicleInfo*> m_VehicleInfos;

	static bool HasVehicle(CVehicle* veh);
	static VehicleInfo* AddVehicle(CVehicle* veh);
	static int GetVehicleId(CVehicle* veh);
	static VehicleInfo* GetVehicle(CVehicle* veh);
	static void RemoveVehicle(CVehicle* veh);
	static void ProccessVehicles();

	CVehicle* m_Vehicle;
	int m_nCreatedAt;
	int m_nId;

	int m_nRenderTime = 0;



	int m_nLastChangedPattern = 0;
	int m_nCurrentPattern = 0;

	std::vector<int> m_CurrentFrame;
	std::vector<int> m_LastChangedFrame;


	void ResetSirenFrames();
	void Draw();
	void VehicleInfo::DrawLight(CVehicle* atVehicle, Light light);
	void Render();
	void Update();
	void UpdatePattern();

	VehicleInfo(CVehicle* vehicle);
};