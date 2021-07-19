#include "VehicleInfo.h"

#include "SirenData.h"

#include "LightPattern.h"
#include "Menu.h"

float GetAngleBetweenVectors(CVector v1, CVector v2, CVector v3) {
	float v12 = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	float v13 = sqrt(pow(v1.x - v3.x, 2) + pow(v1.y - v3.y, 2));
	float v23 = sqrt(pow(v2.x - v3.x, 2) + pow(v2.y - v3.y, 2));
	return acos((pow(v12, 2) + pow(v13, 2) - pow(v23, 2)) / (2 * v12 * v13));
}

std::map<int, VehicleInfo*> VehicleInfo::m_VehicleInfos;

int lastToggledLights = 0;

bool VehicleInfo::HasVehicle(CVehicle* veh)
{
	int vehicleId = GetVehicleId(veh);

	if (m_VehicleInfos.find(vehicleId) != m_VehicleInfos.end()) return true;

	return false;
}

int VehicleInfo::GetVehicleId(CVehicle* veh)
{
	return reinterpret_cast<int>(veh);
}

VehicleInfo* VehicleInfo::GetVehicle(CVehicle* veh)
{
	int vehicleId = GetVehicleId(veh);

	return m_VehicleInfos[vehicleId];
}

VehicleInfo* VehicleInfo::AddVehicle(CVehicle* veh)
{
	VehicleInfo* vehicleInfo = new VehicleInfo(veh);

	int vehicleId = GetVehicleId(veh);

	m_VehicleInfos[vehicleId] = vehicleInfo;

	return vehicleInfo;
}

void VehicleInfo::RemoveVehicle(CVehicle* veh)
{
	std::map<int, VehicleInfo*>::iterator it;

	for (it = m_VehicleInfos.begin(); it != m_VehicleInfos.end(); ++it)
	{
		if ((*it).second->m_Vehicle == veh)
		{
			break;
		}
	}

	m_VehicleInfos.erase(it);
}

void VehicleInfo::ProccessVehicles()
{
	for (auto pair : m_VehicleInfos)
	{
		VehicleInfo* vehicleInfo = pair.second;

		bool found = false;

		for (CVehicle* v : CPools::ms_pVehiclePool)
		{
			if (v == vehicleInfo->m_Vehicle)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			VehicleInfo::RemoveVehicle(vehicleInfo->m_Vehicle);

		}
	}

	for (CVehicle* vehicle : CPools::ms_pVehiclePool)
	{
		if (!HasVehicle(vehicle))
		{
			if (SirenData::DoesModelHaveData(vehicle->m_nModelIndex))
			{
				VehicleInfo* vehicleInfo = AddVehicle(vehicle);
			}

			

		}
		else {


			//if (!VehicleInfo::HasVehicle(vehicle)) return;

			VehicleInfo* vehicleInfo = VehicleInfo::GetVehicle(vehicle);

			if (!SirenData::DoesModelHaveData(vehicle->m_nModelIndex))
			{
				//flog << "Different car, same id. Removing " << vehicleInfo->m_nId << std::endl;

				VehicleInfo::RemoveVehicle(vehicle);
			}
			else {
				if (CTimer::m_snTimeInMilliseconds - vehicleInfo->m_nRenderTime < 200)
				{
					vehicleInfo->Update();
					vehicleInfo->Render();
				}

				
			}

			//VehicleInfo* vehicleInfo = GetVehicle(vehicle);

			//flog << "Updating " << vehicleInfo->m_nId << std::endl;

			//vehicleInfo->Update();
			//update
		}
	}



	//----
	CVehicle* vehicle = FindPlayerVehicle(0, false);

	if (vehicle)
	{
		if (KeyPressed(VK_LCONTROL) && KeyPressed(74) && CTimer::m_snTimeInMilliseconds - lastToggledLights > 300)
		{
			lastToggledLights = CTimer::m_snTimeInMilliseconds;


			if (HasVehicle(vehicle))
			{
				VehicleInfo* vehicleInfo = VehicleInfo::GetVehicle(vehicle);
				vehicleInfo->m_bEnabled = !vehicleInfo->m_bEnabled;
			}
		}
	}
}

VehicleInfo::VehicleInfo(CVehicle* vehicle)
{
	m_Vehicle = vehicle;
	m_nCreatedAt = CTimer::m_snTimeInMilliseconds;
	m_nId = GetVehicleId(vehicle);

	Update();
}

void VehicleInfo::Draw()
{
	/*
	
	int modelId = m_Vehicle->m_nModelIndex;

	CVector& posn = m_Vehicle->GetPosition();

	RwV3d rwp = { posn.x, posn.y, posn.z };
	RwV3d screenCoors; float w, h;

	CFont::SetOrientation(ALIGN_CENTER);
	CFont::SetColor(CRGBA(0, 255, 0, 255));
	CFont::SetDropShadowPosition(1);
	CFont::SetBackground(false, false);
	CFont::SetWrapx(500.0);
	CFont::SetScale(0.5, 1.0);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetProportional(true);
	CSprite::CalcScreenCoors(rwp, &screenCoors, &w, &h, true, true);

	SirenData* data = SirenData::GetDataOfModel(modelId);
	int numSirens = data->m_Data["siren"].size();


	char text[256];
	sprintf(text, "pattern %d", m_nCurrentPattern);
	CFont::PrintString(screenCoors.x, screenCoors.y, text);

	*/
}

void VehicleInfo::ResetSirenFrames()
{
	for (int i = 0; i < (int)m_CurrentFrame.size(); i++) m_CurrentFrame[i] = 0;
	for (int i = 0; i < (int)m_LastChangedFrame.size(); i++) m_LastChangedFrame[i] = CTimer::m_snTimeInMilliseconds;
}

void VehicleInfo::UpdatePattern()
{
	int modelId = m_Vehicle->m_nModelIndex;

	SirenData* data = SirenData::GetDataOfModel(modelId);

	int numSirens = data->m_Data["siren"].size();

	if ((int)m_CurrentFrame.size() > numSirens)
	{
		m_CurrentFrame.clear();
		m_LastChangedFrame.clear();
	}

	while ((int)m_CurrentFrame.size() < numSirens)
	{
		m_CurrentFrame.push_back(0);
		m_LastChangedFrame.push_back(0);
	}


	if (CTimer::m_snTimeInMilliseconds - m_nLastChangedPattern > 8000)
	{
		m_nLastChangedPattern = CTimer::m_snTimeInMilliseconds;
		m_nCurrentPattern++;

		ResetSirenFrames();

		if (m_nCurrentPattern >= LightPatterns::GetTotalPatterns()) m_nCurrentPattern = 0;
	}

	for (int sirenIndex = 0; sirenIndex < numSirens; sirenIndex++)
	{
		int patternOffset = data->m_Data["siren"][sirenIndex]["pattern"];
		int currentPattern = (m_nCurrentPattern + patternOffset) % LightPatterns::GetTotalPatterns();

		LightPattern* pattern = LightPatterns::GetPattern(currentPattern);

		if (m_CurrentFrame[sirenIndex] >= pattern->GetTotalFrames()) m_CurrentFrame[sirenIndex] = 0;

		LightPatternFrame frame = pattern->GetFrame(m_CurrentFrame[sirenIndex]);

		if ((int)CTimer::m_snTimeInMilliseconds - m_LastChangedFrame[sirenIndex] >= frame.duration)
		{
			m_LastChangedFrame[sirenIndex] = CTimer::m_snTimeInMilliseconds;

			m_CurrentFrame[sirenIndex]++;

			if (m_CurrentFrame[sirenIndex] >= pattern->GetTotalFrames()) m_CurrentFrame[sirenIndex] = 0;
		}
	}
}

void VehicleInfo::Render()
{


	int id = reinterpret_cast<unsigned int>(m_Vehicle);

	int modelId = m_Vehicle->m_nModelIndex;

	SirenData* data = SirenData::GetDataOfModel(modelId);

	int numSirens = data->m_Data["siren"].size();

	for (int sirenIndex = 0; sirenIndex < numSirens; sirenIndex++)
	{
		int patternOffset = data->m_Data["siren"][sirenIndex]["pattern"];
		int currentPattern = (m_nCurrentPattern + patternOffset) % LightPatterns::GetTotalPatterns();

		LightPattern* pattern = LightPatterns::GetPattern(currentPattern);

		LightPatternFrame frame = pattern->GetFrame(m_CurrentFrame[sirenIndex]);

		nlohmann::json sirenData = data->m_Data["siren"][sirenIndex];

		if ((bool)sirenData["disabled"]) continue;

		int numLights = sirenData["number_of_lights"];

		for (int lightIndex = 0; lightIndex < numLights; lightIndex++)
		{
			nlohmann::json lightData = sirenData["light"][lightIndex];

			Light light;

			light.id = id;

			id++;


			float distanceBetweenLights = (float)sirenData["distance_between_lights"];

			float posx = (lightIndex)*distanceBetweenLights - ((numLights - 1) * distanceBetweenLights / 2);

			light.radius = (float)sirenData["radius"];
			light.nearClip = (float)sirenData["near_clip"];

			light.offset.x = (float)sirenData["position"]["x"] + posx;
			light.offset.y = (float)sirenData["position"]["y"];
			light.offset.z = (float)sirenData["position"]["z"];

			light.dontShowAreaLight = !((bool)sirenData["use_area_light"]);

			light.type = (eCoronaType)sirenData["type"];

			light.enabled = false;



			LightPosition position = LightPosition::LEFT;





			if (lightIndex < floor(numLights / 2)) position = LightPosition::LEFT;
			if (lightIndex >= floor(numLights / 2) + (numLights % 2 == 0 ? 0 : 1)) position = LightPosition::RIGHT;
			if (numLights == 1) position = LightPosition::MIDDLE;

			if (position == LightPosition::LEFT && frame.leftLightState) light.enabled = true;
			if (position == LightPosition::RIGHT && frame.rightLightState) light.enabled = true;
			if (position == LightPosition::MIDDLE && frame.middleLightState) light.enabled = true;

			if (Menu::m_bFreezeLights && Menu::m_bMenuVisible) light.enabled = true;

			if (position == LightPosition::RIGHT)
			{
				light.r = (float)sirenData["color_secondary"]["r"] * 255.0f;
				light.g = (float)sirenData["color_secondary"]["g"] * 255.0f;
				light.b = (float)sirenData["color_secondary"]["b"] * 255.0f;
			}
			else {
				light.r = (float)sirenData["color_primary"]["r"] * 255.0f;
				light.g = (float)sirenData["color_primary"]["g"] * 255.0f;
				light.b = (float)sirenData["color_primary"]["b"] * 255.0f;
			}

			LightDirection direction = (LightDirection)sirenData["direction"];



			//
			CVector lightWorldPos = m_Vehicle->TransformFromObjectSpace(light.offset);

			float dir = (float)GetAngleBetweenVectors(m_Vehicle->GetPosition(), lightWorldPos, TheCamera.GetPosition());

			if (isnan(dir)) dir = 0.01f;

			const float start_fadeout = 1.00f;
			const float end_fadeout = 1.60f;

			if ((light.offset.y >= 0.1f || light.offset.y <= -0.1f) && direction != LightDirection::ALL)
			{
				if (dir >= start_fadeout && dir <= end_fadeout)
				{
					float r = (dir - start_fadeout) / (end_fadeout - start_fadeout);
					light.radius *= (1.0f - r);
				}

				if (dir >= end_fadeout && direction == LightDirection::ALL) light.dontShowCorona = true;

				if (direction == LightDirection::FRONT_OR_BACK && dir >= end_fadeout) light.dontShowCorona = true;
			}
			//

			if (!m_bEnabled && !Menu::m_bMenuVisible) light.enabled = false;

			DrawLight(m_Vehicle, light);
		}
	}
}

void VehicleInfo::Update()
{
	//flog << "Updating " << m_nId << std::endl;

	bool currentSirenState = m_Vehicle->m_nVehicleFlags.bSirenOrAlarm;
	
	if (currentSirenState != m_bPrevSirenState)
	{
		m_bPrevSirenState = m_Vehicle->m_nVehicleFlags.bSirenOrAlarm;
		m_bEnabled = currentSirenState;
	}

	

	UpdatePattern();
}

void VehicleInfo::DrawLight(CVehicle* atVehicle, Light light)
{
	if (!light.dontShowCorona)
	{
		if (!light.enabled) light.a = 0;

		if (light.a == 0) light.radius = 0;

		light.a = 255;

		CCoronas::RegisterCorona(
			light.id,
			atVehicle,
			light.r,
			light.g,
			light.b,
			light.a,
			light.offset,
			light.radius,
			light.farClip,
			light.type,
			light.flareType,
			false,
			false,
			0,
			0.0f,
			false,
			light.nearClip,
			0,
			15.0f,
			false,
			false
		);
	}

	


	//
	if (!light.enabled || light.dontShowAreaLight) return;

	CVector position = atVehicle->TransformFromObjectSpace(light.offset);

	Command< 0x09E5 >(position.x, position.y, position.z, light.r, light.g, light.b, light.flashDistance);
	//
}