#include "MenuSiren.h"
#include "SirenData.h"
#include "Menu.h"
#include "LightPattern.h"



MenuSiren::MenuSiren(int siren)
{
	m_nSiren = siren;

	SirenData* data = SirenData::GetDataOfModel(Menu::m_nModelId);
	nlohmann::json sirenData = data->m_Data["siren"][siren];

	m_NumLights.AddOption(1, "1");
	m_NumLights.AddOption(2, "2");
	m_NumLights.AddOption(4, "4");
	m_NumLights.AddOption(6, "6");
	m_NumLights.SetValue(sirenData["number_of_lights"]);

	m_LightType.AddOption(0, "Tipo 1");
	m_LightType.AddOption(2, "Tipo 2");
	m_LightType.AddOption(3, "Tipo 3");
	m_LightType.AddOption(4, "Tipo 4");
	m_LightType.AddOption(9, "Tipo 5");
	m_LightType.SetValue(sirenData["type"]);

	//

	

	for (int i = 0; i < LightPatterns::GetTotalPatterns(); i++)
	{
		char text[256];
		sprintf(text, "Padrao %d", i + 1);

		m_Pattern.AddOption(i, text);
		
	}

	m_Pattern.SetValue(sirenData["pattern"]);
	//

	m_Direction.AddOption(0, "Todas as direcoes");
	m_Direction.AddOption(1, "Frente / tras");
	m_Direction.SetValue(sirenData["direction"]);


	m_fPositionX = sirenData["position"]["x"];
	m_fPositionY = sirenData["position"]["y"];
	m_fPositionZ = sirenData["position"]["z"];
	

	std::string name = data->m_Data["siren"][m_nSiren]["name"].get<std::string>();
	sprintf(m_Name, "%s", name.c_str());


	m_PrimaryColor[0] = (float)sirenData["color_primary"]["r"];
	m_PrimaryColor[1] = (float)sirenData["color_primary"]["g"];
	m_PrimaryColor[2] = (float)sirenData["color_primary"]["b"];

	m_SecondaryColor[0] = (float)sirenData["color_secondary"]["r"];
	m_SecondaryColor[1] = (float)sirenData["color_secondary"]["g"];
	m_SecondaryColor[2] = (float)sirenData["color_secondary"]["b"];

	m_fDistanceBetweenLights = (float)sirenData["distance_between_lights"];
	m_fNearClip = (float)sirenData["near_clip"];
	m_fRadius = (float)sirenData["radius"];

	m_bUseAreaLight = (bool)sirenData["use_area_light"];
	m_bDisabled = (bool)sirenData["disabled"];
}

void MenuSiren::Draw()
{
	ImGui::Begin("Editando sirene");
	
	if (_setsize)
	{
		ImGui::SetWindowSize(ImVec2(400, 460));
		_setsize = false;
	}

	if (ImGui::Button("Voltar"))
	{
		Menu::m_ListMenuSiren.clear();
		Menu::m_bShowMainMenu = true;
	}


	ImGui::Text("Segure ALT para mover a camera");

	ImGui::Checkbox("Desativar", &m_bDisabled);

	ImGui::InputText("Nome", m_Name, 256);

	if (ImGui::BeginCombo("Quantidade de luzes", m_NumLights.GetValueDisplay().c_str(), ImGuiComboFlags_NoArrowButton))
	{
		int i = 0;
		for (std::string item : m_NumLights.GetDisplayOptions())
		{
			bool is_selected = m_NumLights.GetSelection() == i;

			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				m_NumLights.SetSelection(i);
			}

			i++;
		}
			
		ImGui::EndCombo();
	}

	ImGui::SliderFloat("Tamanho", &m_fRadius, 0.0f, 8.0f);

	ImGui::SliderFloat("Distancia entre luzes", &m_fDistanceBetweenLights, 0.0f, 8.0f);

	if (ImGui::BeginCombo("Direcao das luzes", m_Direction.GetValueDisplay().c_str(), ImGuiComboFlags_NoArrowButton))
	{
		int i = 0;
		for (std::string item : m_Direction.GetDisplayOptions())
		{
			bool is_selected = m_Direction.GetSelection() == i;

			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				m_Direction.SetSelection(i);
			}

			i++;
		}

		ImGui::EndCombo();
	}

	if (ImGui::BeginCombo("Padrao das luzes", m_Pattern.GetValueDisplay().c_str(), ImGuiComboFlags_NoArrowButton))
	{
		int i = 0;
		for (std::string item : m_Pattern.GetDisplayOptions())
		{
			bool is_selected = m_Pattern.GetSelection() == i;

			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				m_Pattern.SetSelection(i);
			}

			i++;
		}

		ImGui::EndCombo();
	}

	ImGui::SliderFloat("X / Lados", &m_fPositionX, -8.0f, 8.0f);

	ImGui::SliderFloat("Y / Frente, Tras", &m_fPositionY, -8.0f, 8.0f);

	ImGui::SliderFloat("Z / Altura", &m_fPositionZ, -8.0f, 8.0f);

	ImGui::ColorEdit3("Cor primaria", m_PrimaryColor);

	ImGui::ColorEdit3("Cor secundaria", m_SecondaryColor);

	if (ImGui::BeginCombo("Tipo da luz", m_LightType.GetValueDisplay().c_str(), ImGuiComboFlags_NoArrowButton))
	{
		int i = 0;
		for (std::string item : m_LightType.GetDisplayOptions())
		{
			bool is_selected = m_LightType.GetSelection() == i;

			if (ImGui::Selectable(item.c_str(), is_selected))
			{
				m_LightType.SetSelection(i);
			}

			i++;
		}

		ImGui::EndCombo();
	}
	
	ImGui::Checkbox("Congelar luzes durante edicao", &Menu::m_bFreezeLights);

	ImGui::SliderFloat("Near Clip", &m_fNearClip, 0.0f, 2.0f);
	
	ImGui::Checkbox("Iluminar ambiente", &m_bUseAreaLight);

	SirenData* data = SirenData::GetDataOfModel(Menu::m_nModelId);
	data->m_Data["siren"][m_nSiren]["number_of_lights"] = m_NumLights.GetValue();

	data->m_Data["siren"][m_nSiren]["type"] = m_LightType.GetValue();

	data->m_Data["siren"][m_nSiren]["pattern"] = m_Pattern.GetValue();

	data->m_Data["siren"][m_nSiren]["direction"] = m_Direction.GetValue();

	data->m_Data["siren"][m_nSiren]["position"]["x"] = m_fPositionX;
	data->m_Data["siren"][m_nSiren]["position"]["y"] = m_fPositionY;
	data->m_Data["siren"][m_nSiren]["position"]["z"] = m_fPositionZ;

	data->m_Data["siren"][m_nSiren]["color_primary"]["r"] = m_PrimaryColor[0];
	data->m_Data["siren"][m_nSiren]["color_primary"]["g"] = m_PrimaryColor[1];
	data->m_Data["siren"][m_nSiren]["color_primary"]["b"] = m_PrimaryColor[2];

	data->m_Data["siren"][m_nSiren]["color_secondary"]["r"] = m_SecondaryColor[0];
	data->m_Data["siren"][m_nSiren]["color_secondary"]["g"] = m_SecondaryColor[1];
	data->m_Data["siren"][m_nSiren]["color_secondary"]["b"] = m_SecondaryColor[2];

	data->m_Data["siren"][m_nSiren]["distance_between_lights"] = m_fDistanceBetweenLights;
	data->m_Data["siren"][m_nSiren]["near_clip"] = m_fNearClip;
	data->m_Data["siren"][m_nSiren]["radius"] = m_fRadius;

	data->m_Data["siren"][m_nSiren]["use_area_light"] = m_bUseAreaLight;
	data->m_Data["siren"][m_nSiren]["disabled"] = m_bDisabled;

	data->m_Data["siren"][m_nSiren]["name"] = m_Name;

	if (ImGui::Button("Excluir sirene"))
	{
		data->m_Data["siren"].erase(m_nSiren);

		Menu::Close();
		Menu::m_bMenuVisible = true;
	}
}