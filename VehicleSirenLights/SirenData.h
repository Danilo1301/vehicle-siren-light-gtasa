#pragma once

#include "pch.h"

class SirenData
{
	static std::map<int, SirenData*> m_ModelsData;

public:
	nlohmann::json m_Data;
	int m_nModelId;

	void StoreData();
	int CreateSiren();
	//int CreateLight(int siren);

	SirenData(int modelId);

	static void LoadSirens();

	static SirenData* GetDataOfModel(int modelId);
	static SirenData* CreateDataForModel(int modelId);
	static bool DoesModelHaveData(int modelId);
};