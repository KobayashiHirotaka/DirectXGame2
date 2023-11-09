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

	void BehaviorDriftInitialize();

	void BehaviorDriftUpdate();

	void BehaviorJumpInitialize();

	void BehaviorJumpUpdate();

	Weapon* GetWeapon() { return weapon_.get(); };

private:
	enum class Behavior
	{
		kRoot,
		kAttack,
		kDrift,
		kJump
	};

	struct WorkDash
	{
		uint32_t dashParameter_= 0;
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

	WorkDash workDrift_;

	std::unique_ptr<Weapon> weapon_ = nullptr;

	uint32_t behaviorDashTime_ = 15;

	Vector3 rotationAmount_;

	Vector3 move_;

	float kJumpFirstSpeed_ = 1.0f;

	Vector3 velocity_ = {};

	float runSpeed_ = 1.5f; // 速い走行速度（調整可能）

	bool isHit_ = false;
	bool preIsHit_ = false;

	bool reStart_ = false;

	bool isDrifting = false;
};

