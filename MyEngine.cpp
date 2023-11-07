#include "MyEngine.h"
#include "SceneManager.h"

MyEngine* MyEngine::GetInstance()
{
	static MyEngine instance;
	return &instance;
}

void MyEngine::Initialize()
{
	win_ = WindowsApp::GetInstance();
	win_->Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	dxCommon_->Initialize(win_);

	textureManager_ = TextureManager::GetInstance();
	textureManager_->Initialize(dxCommon_);
}

void MyEngine::ImGui()
{
	ImGui::ShowDemoWindow();
}

