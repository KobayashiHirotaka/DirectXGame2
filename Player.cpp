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

		case Behavior::kJump:
			BehaviorJumpInitialize();
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

	case Behavior::kJump:
		BehaviorJumpUpdate();
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
	worldTransform_.Initialize();
}

void Player::BehaviorRootUpdate()
{
	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		behaviorRequest_ = Behavior::kDrift;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		behaviorRequest_ = Behavior::kJump;
	}

	if (!isHit_) {
		worldTransform_.translation.y -= 0.05f;
	}

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
		worldTransform_.translation.y += 0.1f;
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

			Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->rotation);

			velocity_ = TransformNormal(velocity_, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, velocity_);

			if (velocity_.z == 0.0 && velocity_.x == 0.0)
			{
				targetAngle_ = 0.0;

			}
			else {
				float dotProduct = velocity_.x;

				float magnitude = std::sqrt(velocity_.x * velocity_.x + velocity_.z * velocity_.z);

				targetAngle_ = std::acos(dotProduct / magnitude);

				if (velocity_.z < 0)
				{
					targetAngle_ = -targetAngle_;
				}
			}

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
	float rotationSpeed = 0.1f;

	if (input_->GetJoystickState(joyState_) && isDrifting == false)
	{
		rotationAmount_ = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX,0.0f, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };
		worldTransform_.rotation.y += rotationSpeed;

		// Aボタンが離されたらドリフトモードを解除
		if (!(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B))
		{
			isDrifting = true;
		}
		
	}

	if (isDrifting)
	{
		workDrift_.dashParameter_++;
		const float deadZone = 0.0f;

		move_ = { rotationAmount_.x, 0.0f, rotationAmount_.z };

		bool isMoving = false;

		if (Length(move_) > deadZone)
		{
			isMoving = true;
		}

		if (isMoving)
		{
			move_ = Multiply(runSpeed_, Normalize(move_));

			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
			move_ = TransformNormal(move_, rotateMatrix);

			worldTransform_.translation = Add(worldTransform_.translation, move_);
		}
		//else
		//{
		//	// スティックを離した後も以前の角度を使って進む
		//	move = Multiply(runSpeed_, Normalize(move));

		//	Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation.y);
		//	move = TransformNormal(move, rotateMatrix);

		//	worldTransform_.translation = Add(worldTransform_.translation, move);
		//}
	}

	if (workDrift_.dashParameter_ >= behaviorDashTime_)
	{
		isDrifting = false;
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorJumpInitialize()
{
	isHit_ = false;

	velocity_ = { (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, kJumpFirstSpeed_, (float)joyState_.Gamepad.sThumbLY / SHRT_MAX };
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

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "Player";

	playerSpeed_ = globalVariables->GetFloatValue(groupName, "Speed");
}
