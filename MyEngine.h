#pragma once
#include "WindowsApp.h"
#include"DirectXCommon.h"
#include "TextureManager.h"

#include <fstream>
#include <numbers>

class MyEngine
{
public:
	static MyEngine* GetInstance();

	//MyEngine();
	//~MyEngine();

	void Initialize();

	void ImGui();

private:
	WindowsApp* win_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	HRESULT hr_;
};
