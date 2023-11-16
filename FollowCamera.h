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

	void SetStickFixed(bool rightFixed, bool leftFixed) {
		stickRightFixed_ = rightFixed;
		stickLeftFixed_ = leftFixed;
	};

private:
	WorldTransform worldTransform_;
	const WorldTransform* target_ = nullptr;

	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	Vector3 interTarget_{};

	Vector3 offset = { 0.0f, 2.0f, -60.0f };

	float destinationAngleY_ = 0.0f;

	bool stickRightFixed_ = false;
	bool stickLeftFixed_ = false;
};

