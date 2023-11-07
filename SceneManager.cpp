#include "SceneManager.h"
#include "GlobalVariables.h"

void SceneManager::Run()
{
	Initialize();
	Update();
	Release();
}

void SceneManager::Initialize()
{
	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	win_ = WindowsApp::GetInstance();

	dxCommon_ = DirectXCommon::GetInstance();

	engine_ = MyEngine::GetInstance();
	engine_->Initialize();

	textureManager_ = TextureManager::GetInstance();

	imGui_ = MyImGui::GetInstance();
	imGui_->Initialize(win_, dxCommon_);

	input_ = Input::GetInstance();
	input_->Initialize(win_);

	audio_ = Audio::GetInstance();
	audio_->Initialize();

	light_ = Light::GetInstance();
	light_->Initialize();

	GlobalVariables::GetInstance()->LoadFiles();

	state[START] = std::make_unique<GameStartScene>();
	state[PLAY] = std::make_unique<GamePlayScene>();
	state[PLAY]->Initialize();

	IScene::sceneNum = PLAY;
}

void SceneManager::Update()
{
	//ウィンドウのxが押されるまでループ
	while (msg.message != WM_QUIT)
	{
		//windowのメッセージを最優先で処理させる
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}else {
			//ゲームの処理
			imGui_->BeginFlame();
			dxCommon_->PreDraw();
			input_->Update();
			GlobalVariables::GetInstance()->Update();

			state[GameStartScene::sceneNum]->Update();
			state[GameStartScene::sceneNum]->Draw();

			imGui_->EndFlame();
			dxCommon_->PostDraw();
		}
	}
}

void SceneManager::Release()
{
	dxCommon_->Release();

	ImGui_ImplDX12_Shutdown();
	CoUninitialize();
}