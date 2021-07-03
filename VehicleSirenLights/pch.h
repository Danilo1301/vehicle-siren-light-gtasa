#pragma once
#pragma warning(disable:4503 4244 4005)

//#define INPUT_BUFFER_SIZE 64
//#define SPAWN_PED_LIMIT 20
//#define STB_IMAGE_IMPLEMENTATION
//#define DISCORD_INVITE "https://discord.gg/ZzW7kmf"
//#define GITHUB_LINK "https://github.com/user-grinch/Cheat-Menu"

#include <d3d9.h>
#include <d3d11.h>
#include <d3d11Shader.h>
#include <D3dx9tex.h>
#include <D3DX11tex.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>

#include "plugin.h"
#include "CBike.h"
#include "CFont.h"
#include "CSprite.h"
#include "CCamera.h"
#include "CCheat.h"
#include "CClothes.h"
#include "CCoronas.h"
#include "CMessages.h"
#include "CClock.h"
#include "CCivilianPed.h"
#include "CCutsceneMgr.h"
#include "CGangs.h"
#include "CGangWars.h"
#include "cHandlingDataMgr.h"
#include "CHud.h"
#include "CMenuManager.h"
#include "CModelInfo.h"
#include "CRadar.h"
#include "RenderWare.h"
#include "CShadows.h"
#include "CStats.h"
#include "CStreaming.h"
#include "CTheScripts.h"
#include "CTheZones.h"
#include "CTimer.h"
#include "CTimeCycle.h"
#include "CTrain.h"
#include "CWeather.h"
#include "CWorld.h"
#include "extensions/ScriptCommands.h"
#include "extensions/Screen.h"
#include "eVehicleClass.h"
#include "extensions/Paths.h"

#include "../Depend/fla/IDaccess.h"
#include "../Depend/imgui/imgui.h"
#include "../Depend/imgui/imgui_internal.h"
#include "../Depend/imgui/imgui_impl_dx9.h"
#include "../Depend/imgui/imgui_impl_dx11.h"
#include "../Depend/imgui/imgui_impl_win32.h"

#include "../Depend/json.hpp"

#include "Hook.h"

#include "extensions/ScriptCommands.h"

// Globals
using CallbackTable = std::vector<std::pair<std::string, void(*)()>>;
using namespace plugin;
namespace fs = std::filesystem;

enum Renderer
{
	Render_DirectX9,
	Render_DirectX11,
	Render_Unknown
};

struct Globals
{
	//inline static std::string m_HeaderId;
	inline static ImVec2 m_fMenuSize = ImVec2(screen::GetScreenWidth() / 4, screen::GetScreenHeight() / 1.2);
	inline static ImVec2 m_fScreenSize = ImVec2(-1, -1);
	//inline static bool m_bShowMenu = false;
	inline static bool m_bInit;
	inline static Renderer renderer = Render_Unknown;
	inline static void* device;
	inline static bool m_bIsSamp = false;
};

//extern std::ofstream flog;

extern void ShowMessageBox(const char* text);