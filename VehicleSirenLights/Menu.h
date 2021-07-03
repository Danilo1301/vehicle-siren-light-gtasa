#pragma once

#include "pch.h"

#include "MenuSiren.h"

class Menu
{
private:
public:
	static std::vector<MenuSiren*> m_ListMenuSiren;

	static bool m_bMenuVisible;
	static bool m_bShowMainMenu;
	static bool m_bFreezeLights;
	static int m_nModelId;

	static void StartEditSiren(int siren);
	static void Update();
	static void Draw();
	static void Close();
	static void DrawMainMenu();
};