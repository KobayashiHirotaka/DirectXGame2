#include "Player.h"
#include "GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	input_ = Input::GetInstance();

	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize(models_[1]);
	weapon_->SetParent(&worldTransform_);

	worldTransform_.translation.y = 1.0f;

	behaviorDashTime_ = 15;

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	float s = 10;
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Speed", playerSpeed_);

	AABB aabbSize =
	{
		{-1.0f,-1.0f,-1.0f},
		{1.0f,1.0f,1.0f},
	};

	SetAABB(aabbSize);
}

void Player::Update()
{
	/*if (preIsHit_ == false && isHit_ == true) 
	{
		worldTransform_.SetParent(parent_);
	}*/

	if (worldTransform_.translation.y < 0.0f) {
		Restart();
	}

	if (preIsHit_ == true && isHit_ == false) 
	{
		worldTransform_.DeleteParent();
	}

	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
	}

	float normalizedX = static_cast<float>(joyState_.Gamepad.sThumbLX) / SHRT_MAX;

	float threshold = 0.5f;

	if (normalizedX > threshold)
	{
		isRightStickRight = true;

	}
	else {
		isRightStickRight = false;
	}


	if (normalizedX < -threshold)
	{
		isRightStickLeft = true;

	}
	else {
		isRightStickLeft = false;
	}

	if (behaviorRequest_)
	{
		behavior_ = behaviorRequest_.value();

		switch (behavior_)
		{
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;

		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;

		case Behavior::kDash:
			BehaviorDashInitialize();
			break;

		case Behavior::kJump:
			BehaviorJumpInitialize();
			break;

		case Behavior::kDrift:
			BehaviorDriftInitialize();
			break;
		}

		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_)
	{
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;

	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;

	case Behavior::kDash:
		BehaviorDashUpdate();
		break;

	case Behavior::kJump:
		BehaviorJumpUpdate();
		break;

	case Behavior::kDrift:
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickRight == true ||
			(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickLeft == true)
		{
			kSpeed -= 0.008f;

			if (kSpeed <= 0.01f)
			{
				kSpeed = 0.01f;
			}

		}
		BehaviorDriftUpdate();

		break;
	}

	/*if (isHit_ == false)
	{
		worldTransform_.translation.y -= 0.1f;
	} */


	/*if (worldTransform_.translation.y <= -4.0f)
	{
		worldTransform_.translation = { 0.0f,0.0f,0.0f };
	}*/

	worldTransform_.quaternion = Slerp(worldTransform_.quaternion, moveQuaternion_, 0.2f);
	worldTransform_.quaternion = Normalize(worldTransform_.quaternion);
	worldTransform_.UpdateMatrix(RotationType::Quaternion);

	weapon_->Update();

	preIsHit_ = isHit_;
	isHit_ = false;
	reStart_ = false;

	Player::ApplyGlobalVariables();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[0]->Draw(worldTransform_, viewProjection);

	if (behavior_ == Behavior::kAttack)
	{
		weapon_->Draw(viewProjection);
	}
}

void Player::Restart()
{
	reStart_ = true;
	worldTransform_.translation = { 0.0f,1.0f,0.0f };
	worldTransform_.parent_ = nullptr;
}

void Player::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() & kCollisionAttributeGround && reStart_== false)
	{
		isHit_ = true;
		if (worldTransform_.translation.y < collider->GetWorldPosition().y + 1.0f) {
			worldTransform_.translation.y = collider->GetWorldPosition().y + 1.0f;
		}
		/*parent_ = &collider->GetWorldTransform();

		if (worldTransform_.parent_ != parent_)
		{
			worldTransform_.DeleteParent();
			worldTransform_.SetParent(parent_);
		}*/
	}

	/*if (collider->GetCollisionAttribute() & kCollisionAttributeEnemy && reStart_ == false)
	{
		Restart();
	}*/

	if (collider->GetCollisionAttribute() & kCollisionAttributeItem1 && reStart_ == false)
	{
		runSpeed_ = 5.0f;
	}

	if (collider->GetCollisionAttribute() & kCollisionAttributeItem2 && reStart_ == false)
	{
		behaviorDashTime_ = 50;
	}
}

Vector3 Player::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

void Player::BehaviorRootInitialize()
{
	
}

void Player::BehaviorRootUpdate()
{
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		behaviorRequest_ = Behavior::kDash;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		behaviorRequest_ = Behavior::kJump;
	}

	if (!isHit_) {
		worldTransform_.translation.y -= 0.05f;
	}

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		Vector3 velocity_ = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

		if (Length(velocity_) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			velocity_ = Multiply(playerSpeed_, Normalize(velocity_));

			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, velocity_);

			velocity_ = Normalize(velocity_);
			Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, velocity_));
			float dot = Dot({ 0.0f,0.0f,1.0f }, velocity_);
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
		}
	}
}

void Player::BehaviorAttackInitialize()
{
	weapon_->Attack();
}

void Player::BehaviorAttackUpdate()
{
	if (weapon_->GetIsAttack() == false)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorDashInitialize()
{
	workDash_.dashParameter_ = 0;
	workDash_.coolTime = 0;
	worldTransform_.rotation.y = targetAngle_;
	kSpeed = 1.0f;
}

void Player::BehaviorDashUpdate()
{
	//移動量
	Vector3 rote = {
		(float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
		0.0f,
		(float)joyState_.Gamepad.sThumbLY / SHRT_MAX,
	};


	if (input_->GetJoystickState(joyState_))
	{
		move_ = {
		rote.x,
		0.0f,
		rote.z,
		};

		//移動量に速さを反映
		move_ = Multiply(kSpeed, Normalize(move_));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		move_ = TransformNormal(move_, rotateMatrix);

		//移動
		worldTransform_.translation = Add(worldTransform_.translation, move_);

		if((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickRight == true ||
			(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickLeft == true)
		{
			behaviorRequest_ = Behavior::kDrift;
		}
	}

	if (++workDash_.dashParameter_ >= behaviorDashTime_)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpInitialize()
{
	isHit_ = false;

	velocity_ = { (float)joyState_.Gamepad.sThumbLX / (SHRT_MAX * 2), kJumpFirstSpeed_, (float)joyState_.Gamepad.sThumbLY / (SHRT_MAX * 2) };
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

	velocity_ = TransformNormal(velocity_, rotateMatrix);
}

void Player::BehaviorJumpUpdate()
{
	worldTransform_.translation = Add(worldTransform_.translation, velocity_);

	const float kGravityAcceleration_ = 0.05f;

	Vector3 accelerationVector_ = { 0.0f,-kGravityAcceleration_,0.0f };

	velocity_ = Add(velocity_, accelerationVector_);

	if (isHit_)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorDriftInitialize()
{
	kSpeed = 1.0f;

	isRightStickRight = false;

	isRightStickLeft = false;
}

void Player::BehaviorDriftUpdate()
{
	if (isRightStickRight)
	{
		moveQuaternion_.y += rotationSpeed;
	} else if (isRightStickLeft) {
		moveQuaternion_.y -= rotationSpeed;
	}

	ImGui::Begin("speed");
	ImGui::Text("%f", kSpeed);
	ImGui::End();

	//移動量
	Vector3 rote = {
		(float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
		0.0f,
		(float)joyState_.Gamepad.sThumbLY / SHRT_MAX,
	};

	if (input_->GetJoystickState(joyState_))
	{
		move_ = {
		rote.x,
		0.0f,
		rote.z,
		};

		//移動量に速さを反映
		move_ = Multiply(kSpeed, Normalize(move_));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		move_ = TransformNormal(move_, rotateMatrix);

		//移動
		worldTransform_.translation = Add(worldTransform_.translation, move_);
	}

	if (!(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickRight == true ||
		!(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && isRightStickLeft == true)
	{
		workDash_.dashParameter_++;
		float kSpeed = 3.0f;
		//移動量
		Vector3 move = {
			(float)joyState_.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState_.Gamepad.sThumbLY / SHRT_MAX,
		};

		//移動量に速さを反映
		move = Multiply(kSpeed, Normalize(move));

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		move = TransformNormal(move, rotateMatrix);

		//移動
		worldTransform_.translation = Add(worldTransform_.translation, move);

		move = Normalize(move);
		Vector3 cross = Normalize(Cross({ 0.0f,0.0f,1.0f }, move));
		float dot = Dot({ 0.0f,0.0f,1.0f }, move);
		moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
	}

	if (workDash_.dashParameter_ >= behaviorDashTime_)
	{
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	playerSpeed_ = globalVariables->GetFloatValue(groupName, "Speed");
}
