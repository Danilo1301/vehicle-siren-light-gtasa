#pragma once

#include "pch.h"


class Combo
{
	std::vector<int> options;
	std::vector<std::string> displayOptions;

	int m_nIndex = 0;
public:
	int GetValue()
	{
		return options.at(m_nIndex);
	}

	std::vector<std::string> GetDisplayOptions()
	{
		return displayOptions;
	}

	std::string GetValueDisplay()
	{
		return displayOptions.at(m_nIndex);
	}

	void SetSelection(int index)
	{
		m_nIndex = index;
	}

	int GetSelection()
	{
		return m_nIndex;
	}

	void SetValue(int value)
	{
		int n = 0;
		for (int i : options)
		{
			if (i == value)
			{
				SetSelection(n);
				break;
			}
			n++;
		}
	}

	void AddOption(int value, std::string displayText)
	{
		options.push_back(value);
		displayOptions.push_back(displayText);
	}

	Combo()
	{

	}
};


class MenuSiren
{
private:
	bool _setsize = true;

public:
	int m_nSiren = 0;

	char m_Name[256];

	Combo m_NumLights;
	Combo m_LightType;
	Combo m_Pattern;
	Combo m_Direction;

	bool m_bUseAreaLight = true;
	bool m_bDisabled = false;

	float m_fPositionX = 0.0f;
	float m_fPositionY = 0.0f;
	float m_fPositionZ = 0.0f;

	float m_PrimaryColor[3] = { 1, 1, 1 };
	float m_SecondaryColor[3] = { 1, 1, 1 };

	float m_fDistanceBetweenLights = 0.0f;
	float m_fNearClip = 0.0f;
	float m_fRadius = 0.0f;

	void Draw();

	MenuSiren(int siren);
};


/*
void SirenMenu::Draw()
{
	SirenData* data = SirenData::GetDataOfModel(Menu::m_nModelId);

	char title[256];
	sprintf(title, "Editando veiculo %d - Sirene %d", Menu::m_nModelId, m_nSiren);
	ImGui::Begin(title);

	ImGui::Text("SirenMenu");

	int i = ImGui::GetCurrentWindow()->ContentSize.x;

	ImGui::Text("Quantidade de luzes");

	ImGui::SameLine();


	if (ImGui::BeginCombo("##custom combo", "---", ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(m_NumLights_Options); n++)
		{
			bool is_selected = (n == m_NumLights_Value);

			if (ImGui::Selectable(m_NumLights_Options[n], is_selected))
			{
				m_NumLights_Value = n;
			}

			if (is_selected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Selectable("Bee", true))
	{

	}

	ImGui::Text("text");


	ImGui::SameLine();

	if (ImGui::Selectable("Bee", true))
	{

	}



	ImGui::End();

	/*
	nlohmann::json sirenData = data->m_Data["siren"][_editingSiren];

	ImGui::Text("Q");

	if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]);
			if (ImGui::Selectable(items[n], is_selected))
				current_item = items[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}



*/