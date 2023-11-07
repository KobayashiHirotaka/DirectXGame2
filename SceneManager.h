#pragma once
#include "WindowsApp.h"
#include "DirectXCommon.h"
#include "MyEngine.h"
#include "TextureManager.h"
#include "MyImGui.h"
#include "Input.h"
#include "Audio.h"
#include "Light.h"
#include "IScene.h"
#include "GameStartScene.h"
#include "GamePlayScene.h"
#include "GameEndScene.h"

class SceneManager
{
public:
	void Initialize();

	void Update();

	//void Draw();

	void Release();

	void Run();

	enum GameSceneNum
	{
		START,
		PLAY,
		END
	};

private:
	WindowsApp* win_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	MyEngine* engine_ = nullptr;

	MyImGui* imGui_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	Light* light_ = nullptr;

	MSG msg{};

	std::unique_ptr<IScene>state[END];
};