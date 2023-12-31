#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
#include <array>
#include <wrl.h>
#include "WindowsApp.h"

class Input
{
public:
	static Input* GetInstance();

	//Input();
	//~Input();

	void Initialize(WindowsApp* win);

	void Update();

	//押した時
	bool PushKey(uint8_t keyNumber)const;

	//押している間
	bool PressKey(uint8_t keyNumber)const;

	//離した時
	bool IsReleseKey(uint8_t keyNumber)const;

	bool GetJoystickState(XINPUT_STATE& state);

private:
	static Input* input_;

	Microsoft::WRL::ComPtr<IDirectInput8>directInput_ = nullptr;
	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

	std::array<BYTE, 256> keys_;
	std::array<BYTE, 256> preKeys_;

	XINPUT_STATE state_{};
};

