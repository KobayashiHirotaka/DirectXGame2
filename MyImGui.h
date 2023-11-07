#pragma once
#include "WindowsApp.h"
#include "DirectXCommon.h"

class MyImGui
{
public:
	static MyImGui* GetInstance();

	//MyImGui();
	//~MyImGui();

	void Initialize(WindowsApp* win, DirectXCommon* dxCommon);

	void BeginFlame();

	void EndFlame();
};

