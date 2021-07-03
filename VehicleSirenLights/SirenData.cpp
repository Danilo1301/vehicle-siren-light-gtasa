#include "SirenData.h"
#include "LightPattern.h"

std::map<int, SirenData*> SirenData::m_ModelsData;

void SirenData::LoadSirens()
{
	//flog << "Loading config..." << std::endl;

	std::string path = PLUGIN_PATH((char*)"/vehicles/");

	for (const auto& entry : fs::directory_iterator(path))
	{
		std::string fileName = entry.path().filename().string();

		//flog << "Found " << fileName << std::endl;

		std::string strmodel = fileName.substr(0, fileName.find("."));


		try
		{
			int modelid = std::stoi(strmodel);

			nlohmann::json data;

			std::ifstream file(entry.path());
			file >> data;
			file.close();

			if (!SirenData::DoesModelHaveData(modelid))
			{
				SirenData::CreateDataForModel(modelid);
			}

			SirenData* sirenData = SirenData::GetDataOfModel(modelid);

			sirenData->m_Data = data;

			//flog << "Loaded config for model " << modelid << std::endl;
		}
		catch (...)
		{
			//flog << "Error!" << std::endl;
		}
	}
}

SirenData::SirenData(int modelId)
{
	m_nModelId = modelId;

	//m_Data["some_option_for_the_vehicle"] = "and value";
	m_Data["siren"] = nlohmann::json::array();
}

SirenData* SirenData::GetDataOfModel(int modelId)
{
	return m_ModelsData.at(modelId);
}

SirenData* SirenData::CreateDataForModel(int modelId)
{
	SirenData* data = new SirenData(modelId);

	m_ModelsData.insert(std::pair<int, SirenData*>(modelId, data));

	return data;
}

bool SirenData::DoesModelHaveData(int modelId)
{
	return m_ModelsData.find(modelId) != m_ModelsData.end();
}


void SirenData::StoreData()
{
	char fileName[256];
	sprintf(fileName, "%d.json", m_nModelId);

	std::string path = PLUGIN_PATH((char*)"/vehicles/") + std::string(fileName);

	std::ofstream file(path);
	file << m_Data.dump(4, ' ', false, nlohmann::json::error_handler_t::replace) << std::endl;
	file.close();
}

int SirenData::CreateSiren()
{
	int id = m_Data["siren"].size();

	nlohmann::json siren;

	nlohmann::json position;
	position["x"] = 0.0f;
	position["y"] = 0.0f;
	position["z"] = 0.0f;

	nlohmann::json color_primary;
	color_primary["r"] = 1.0f;
	color_primary["g"] = 0.0f;
	color_primary["b"] = 0.0f;

	nlohmann::json color_secondary;
	color_secondary["r"] = 0.0f;
	color_secondary["g"] = 0.0f;
	color_secondary["b"] = 1.0f;
	
	siren["name"] = "";
	siren["color_primary"] = color_primary;
	siren["color_secondary"] = color_secondary;
	siren["position"] = position;
	siren["number_of_lights"] = 2;
	siren["direction"] = 0;
	siren["type"] = 0;
	siren["pattern"] = 0;
	siren["use_area_light"] = true;
	siren["disabled"] = false;
	siren["near_clip"] = 0.8f;
	siren["radius"] = 0.2f;
	siren["distance_between_lights"] = 0.4f;
	
	//siren["light"] = nlohmann::json::array();
	
	m_Data["siren"].push_back(siren);

	return id;
}

/*
int SirenData::CreateLight(int siren)
{
	int id = m_Data["siren"][siren]["light"].size();

	nlohmann::json light;

	nlohmann::json offset;
	offset["x"] = 0.0f;
	offset["y"] = 0.0f;
	offset["z"] = 0.0f;

	nlohmann::json color;
	color["r"] = 255;
	color["g"] = 255;
	color["b"] = 255;

	light["offset"] = offset;
	light["color"] = color;
	light["position"] = LightPosition::LEFT;

	m_Data["siren"][siren]["light"].push_back(light);

	return id;
}
*/