#include "Menu.h"
#include "Hook.h"
#include "SirenData.h"
#include "LightPattern.h"

int _lastOpened = 0;
bool _setsize = true;

std::vector<MenuSiren*> Menu::m_ListMenuSiren;

bool Menu::m_bMenuVisible = false;
bool Menu::m_bShowMainMenu = true;
bool Menu::m_bFreezeLights = false;

int Menu::m_nModelId = 0;

void Menu::Close()
{
	m_bMenuVisible = false;
	Hook::SetMouseVisiblity(false);

	m_ListMenuSiren.clear();

	if (m_nModelId != 0)
	{
		SirenData* data = SirenData::GetDataOfModel(m_nModelId);
		data->StoreData();
	}

	m_bShowMainMenu = true;
}



void Menu::Update()
{
	CVehicle* vehicle = FindPlayerVehicle(0, false);

	if (m_bMenuVisible)
	{
		Hook::SetMouseVisiblity(!KeyPressed(18));
		


		if (vehicle == 0)
		{
			Close();
		}


		
	}

	if (KeyPressed(17) && KeyPressed(76) && CTimer::m_snTimeInMilliseconds - _lastOpened > 300)
	{
		if (Globals::m_bIsSamp)
		{
			CMessages::AddMessageJumpQ("Voce precisa estar no modo Single Player para editar", 2000, 0, false);
			return;
		}

		if (vehicle > 0)
		{
			_lastOpened = CTimer::m_snTimeInMilliseconds;

			int modelId = vehicle->m_nModelIndex;

			if (!SirenData::DoesModelHaveData(modelId)) SirenData* data = SirenData::CreateDataForModel(modelId);
			

			if (modelId != m_nModelId)
			{
				Close();
				
				m_bShowMainMenu = true;
			}

	
			m_nModelId = modelId;
			m_bMenuVisible = !m_bMenuVisible;

			if (!m_bMenuVisible)
			{
				Close();
			}

			

			Hook::SetMouseVisiblity(m_bMenuVisible);

			
		}
		else
		{
			CMessages::AddMessageJumpQ("Voce precisa estar em um veiculo!", 1000, 0, false);
		}
	}
}


void Menu::StartEditSiren(int siren)
{
	m_bShowMainMenu = false;
	MenuSiren* menu = new MenuSiren(siren);
	m_ListMenuSiren.push_back(menu);
}

void Menu::DrawMainMenu()
{

	SirenData* data = SirenData::GetDataOfModel(m_nModelId);

	ImGui::Begin("Vehicle Siren Lights");

	if (_setsize)
	{
		ImGui::SetWindowSize(ImVec2(300, 400));
		_setsize = false;
	}
	
	


	ImGui::Text("Sirenes");

	int numSirens = data->m_Data["siren"].size();

	if (ImGui::Button("Adicionar"))
	{
		int siren = data->CreateSiren();

		data->m_Data["siren"][siren]["position"]["z"] = (Random(0, 100) * 0.01f) + 1.2f;

		data->StoreData();

		StartEditSiren(siren);
		
	}

	ImGui::SameLine();

	if (ImGui::Button("Recarregar config"))
	{
		SirenData::LoadSirens();

		CMessages::AddMessageJumpQ("Configuracao carregada!", 1000, 0, false);
	}

	for (int i = 0; i < numSirens; i++)
	{
		std::string name = std::string(data->m_Data["siren"][i]["name"]);

		char text[256];
		sprintf(text, "[%d] Editar sirene '%s'", i, name.c_str());

		if (ImGui::Button(text))
		{
			StartEditSiren(i);
		}
	}

	ImGui::End();

}



void Menu::Draw()
{
	if (!m_bMenuVisible) return;

	ImGuiIO& io = ImGui::GetIO();


	if (m_bShowMainMenu)
	{
		DrawMainMenu();
	}

	for (auto menu : m_ListMenuSiren)
	{
		menu->Draw();
	}
}



