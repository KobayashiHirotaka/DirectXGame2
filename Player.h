#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "ICharacter.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "MyMath.h"
#include "Weapon.h"
#include <optional>

enum class Behavior
	{
		kRoot,
		kAttack,
		kDash,
		kJump,
		kDrift
	};

class Player : public Collider, public ICharacter
{
public:
	void Initialize(const std::vector<Model*>& models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void OnCollision(Collider* collider)override;

	void SetParent(const WorldTransform* parent) { parent_ = parent; };

	void Restart();

	void ApplyGlobalVariables();

	void BehaviorRootInitialize();

	void BehaviorRootUpdate();

	void BehaviorAttackInitialize();

	void BehaviorAttackUpdate();

	void BehaviorDashInitialize();

	void BehaviorDashUpdate();

	void BehaviorJumpInitialize();

	void BehaviorJumpUpdate();

	void BehaviorDriftInitialize();

	void BehaviorDriftUpdate();

	Weapon* GetWeapon() { return weapon_.get(); };

	Behavior GetBehavior() const {return behavior_;}

private:

	struct WorkDash
	{
		uint32_t dashParameter_= 0;
		uint32_t coolTime = 0;
	};

	struct WorkDrift
	{
		uint32_t driftParameter_ = 0;
		uint32_t coolTime = 0;
	};

	const WorldTransform* parent_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	float playerSpeed_ = 0.4f;

	float targetAngle_ = 0.0f;

	Behavior behavior_ = Behavior::kRoot;

	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	WorkDash workDash_;

	WorkDrift workDrift_;

	std::unique_ptr<Weapon> weapon_ = nullptr;

	uint32_t behaviorDashTime_ = 100;

	uint32_t behaviorDriftTime_ = 100;

	Vector3 rotationAmount_;

	Vector3 move_;

	float kJumpFirstSpeed_ = 1.0f;

	Vector3 velocity_ = {};

	Quaternion moveQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };

	float runSpeed_ = 3.0f; // 速い走行速度（調整可能）

	float kSpeed = 1.0f;

	float rotationSpeed = 0.01f;

	bool isHit_ = false;
	bool preIsHit_ = false;

	bool reStart_ = false;

	bool isDrifting = false;
	bool isCD = false;

	bool isRightStickRight = false;

	bool isRightStickLeft = false;

};

