#include "MyImGui.h"

MyImGui* MyImGui::GetInstance()
{
	static MyImGui instance;
	return &instance;
}

void MyImGui::Initialize(WindowsApp* win, DirectXCommon* dxCommon)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(win->GetHwnd());
	ImGui_ImplDX12_Init(dxCommon->GetDevice().Get(),
		dxCommon->GetSwapChainDesc().BufferCount,
		dxCommon->GetRtvDesc().Format,
		dxCommon->GetSrvDescriptorHeap().Get(),
		dxCommon->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCommon->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void MyImGui::BeginFlame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MyImGui::EndFlame()
{
	ImGui::Render();
}
