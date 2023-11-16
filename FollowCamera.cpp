#include "FollowCamera.h"
#include "GlobalVariables.h"

void FollowCamera::Initialize()
{
	input_ = Input::GetInstance();

	viewProjection_.Initialize();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "FollowCamera";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Offset", offset);
	globalVariables->AddItem(groupName, "rotation", viewProjection_.rotation);
}

void FollowCamera::Update()
{
	if (target_) 
	{
		interTarget_ = Lerp(interTarget_, GetWorldPosition(), 0.1f);
	}

	Vector3 offset = FollowCamera::Offset();
	
	viewProjection_.translation = Add(interTarget_, offset);


	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.2f;

		bool isMoving = false;

		const float kRotSpeedY = 0.04f;

		Vector3 move = { 0.0f, (float)joyState_.Gamepad.sThumbRX / SHRT_MAX, 0.0f };

	/*	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
		{
			Vector3 move = { 0.0f, 0.3f, 0.0f };

			bool isRightStickRight = false;

			bool isRightStickLeft = false;

			float normalizedX = static_cast<float>(joyState_.Gamepad.sThumbLX) / SHRT_MAX;

			float threshold = 0.5f;

			if (normalizedX > threshold)
			{
				isRightStickRight = true;

			}
			else if (normalizedX < -threshold) {
				isRightStickLeft = true;
			}

			if (isRightStickRight)
			{
				destinationAngleY_ += move.y * kRotSpeedY;

			}
			else if (isRightStickLeft) {
				destinationAngleY_ -= move.y * kRotSpeedY;
			}
		}*/

		if (Length(move) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{

			destinationAngleY_ += move.y * kRotSpeedY;
		}
	}

	viewProjection_.rotation.y = LerpShortAngle(viewProjection_.rotation.y, destinationAngleY_, 0.3f);

	viewProjection_.UpdateViewMatrix();
	viewProjection_.TransferMatrix();

	FollowCamera::ApplyGlobalVariables();
}

Vector3 FollowCamera::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = target_->matWorld.m[3][0];
	pos.y = target_->matWorld.m[3][1];
	pos.z = target_->matWorld.m[3][2];
	return pos;
}

void FollowCamera::SetTarget(const WorldTransform* target)
{ 
	target_ = target; 
	FollowCamera::Reset();
}

Vector3 FollowCamera::Offset()
{
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_.rotation.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_.rotation.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}

void FollowCamera::Reset() 
{
	if (target_)
	{
		interTarget_ = target_->translation;
		viewProjection_.rotation.x = target_->rotation.x;
		viewProjection_.rotation.y = target_->rotation.y;
	}

	destinationAngleY_ = viewProjection_.rotation.y;

	Vector3 offset = FollowCamera::Offset();
	viewProjection_.translation = Add(interTarget_, offset);
}

void FollowCamera::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "FollowCamera";

	offset = globalVariables->GetVector3Value(groupName, "Offset");
	viewProjection_.rotation.x = globalVariables->GetVector3Value(groupName, "rotation").x;
	viewProjection_.rotation.z = globalVariables->GetVector3Value(groupName, "rotation").z;
}
