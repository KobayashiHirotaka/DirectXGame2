#pragma once
#include "MyImGui.h"
#include "MyMath.h"
#include "Vector3.h"
#include "Input.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Camera
{
public:
	Camera();

	~Camera();

	void Initialize(int32_t kClientWidth, int32_t kClientHeight);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; };

	void SetTarget(const WorldTransform* target) { target_ = target; };

#ifdef _DEBUG
	void DebugCameraMove();

	void DebugCamera(bool Flag) 
	{
		DebucCameraFlag = Flag;
	}
#endif // DEBUG

private:
	int32_t kClientWidth_;
	int32_t kClientHeight_;

	Input* input_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransform_{};

	//ビュープロジェクション
	ViewProjection viewProjection_{};

	//オフセット
	Vector3 offset_ = { 0.0f,0.0f,-50.0f };

	//累積回転行列
	Matrix4x4 matRot_ = MakeIdentity4x4();

	//追従対象
	const WorldTransform* target_ = nullptr;

#ifdef _DEBUG
	bool DebucCameraFlag = false;

	const float speed = 0.03f;
#endif // DEBUG
};

