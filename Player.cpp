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

	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	float s = 10;
	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Speed", playerSpeed_);
}

void Player::Update()
{
	if (preIsHit_ == false && isHit_ == true) 
	{
		worldTransform_.SetParent(parent_);
	}

	if (preIsHit_ == true && isHit_ == false) 
	{
		worldTransform_.DeleteParent();
	}

	if (!Input::GetInstance()->GetJoystickState(joyState_))
	{
		return;
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

	case Behavior::kDrift:
		BehaviorDriftUpdate();
		break;
	}

	/*if (isHit_ == false)
	{
		worldTransform_.translation.y -= 0.1f;
	} */
	
	if (isHit_ == true)
	{
		worldTransform_.translation.y = 1.0f;
	}

	if (worldTransform_.translation.y <= -4.0f)
	{
		worldTransform_.translation = { 0.0f,0.0f,0.0f };
	}

	ICharacter::Update();

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
	worldTransform_.translation = { 0.0f,0.0f,0.0f };
	worldTransform_.parent_ = nullptr;
}

void Player::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() & kCollisionAttributeGround && reStart_== false)
	{
		isHit_ = true;
		parent_ = &collider->GetWorldTransform();

		if (worldTransform_.parent_ != parent_)
		{
			worldTransform_.DeleteParent();
			worldTransform_.SetParent(parent_);
		}
	}

	if (collider->GetCollisionAttribute() & kCollisionAttributeEnemy && reStart_ == false)
	{
		Restart();
	}

	if (collider->GetCollisionAttribute() & kCollisionAttributeGoal && reStart_ == false)
	{
		Restart();
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
	worldTransform_.Initialize();
}

void Player::BehaviorRootUpdate()
{
	if (workDrift_.coolTime != 60)
	{
		workDrift_.coolTime++;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		if (workDrift_.coolTime == 60)
		{
			isDrifting = true;
			behaviorRequest_ = Behavior::kDrift;
		}
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		if (workDrift_.coolTime == 60)
		{
			behaviorRequest_ = Behavior::kAttack;
		}
	}

	if (Input::GetInstance()->GetJoystickState(joyState_))
	{
		const float deadZone = 0.7f;

		bool isMoving = false;

		Vector3 move = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

		if (Length(move) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			move = Multiply(playerSpeed_, Normalize(move));

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			move = TransformNormal(move, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, move);
			targetAngle_ = std::atan2(move.x, move.z);
		}
	}

	worldTransform_.rotation.y = LerpShortAngle(worldTransform_.rotation.y, targetAngle_, 0.1f);
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

void Player::BehaviorDriftInitialize()
{
	workDrift_.dashParameter_ = 0;
	workDrift_.coolTime = 0;
	worldTransform_.rotation.y = targetAngle_;
}

void Player::BehaviorDriftUpdate()
{
	if (input_->GetJoystickState(joyState_))
	{
		const uint32_t behaviorDashTime = 30;
		float rotationSpeed = 0.1f;

		if (isDrifting)
		{
			// ドリフトモード中
			Vector3 move = { 0.0f, 0.0f, 0.0f }; // ドリフト中は移動しない

			float rotationAmount = (float)joyState_.Gamepad.sThumbLX / SHRT_MAX * rotationSpeed;
			worldTransform_.rotation.y += rotationAmount;

			// Aボタンが離されたらドリフトモードを解除
			if (!(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))
			{
				workDrift_.dashParameter_++;
				const float deadZone = 0.3f;

				bool isMoving = false;

				float runSpeed = 1.5f; // 速い走行速度（調整可能）
				Vector3 move = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

				if (Length(move) > deadZone)
				{
					isMoving = true;
				}

				if (isMoving)
				{
					move = Multiply(runSpeed, Normalize(move));

					Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
					move = TransformNormal(move, rotateMatrix);

					worldTransform_.translation = Add(worldTransform_.translation, move);
				}
			}

			if (workDrift_.dashParameter_ >= behaviorDashTime)
			{
				isDrifting = false;
				behaviorRequest_ = Behavior::kRoot;
			}
		}
	}
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	playerSpeed_ = globalVariables->GetFloatValue(groupName, "Speed");
}
