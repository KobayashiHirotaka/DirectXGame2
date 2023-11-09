#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "MyMath.h"

class FollowCamera
{
public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetTarget(const WorldTransform* target);

	Vector3 GetWorldPosition();

	Vector3 Offset();

	void Reset();

	void ApplyGlobalVariables();

private:
	WorldTransform worldTransform_;
	const WorldTransform* target_ = nullptr;

	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	Vector3 interTarget_{};

	Vector3 offset = { 0.0f, 2.0f, -25.0f };

	float destinationAngleY_ = 0.0f;
};

