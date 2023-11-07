#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <numbers>

class Weapon : public Collider
{
public:
	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform() override { return worldTransform_; };

	Vector3 GetWorldPosition() override;

	void OnCollision(Collider* collider) override;

	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

	void Attack();

	bool GetIsAttack() { return isAttack_; };

private:
	void AttackInitialize();

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	uint16_t attackAnimationCount_ = 0;
	uint16_t attackAnimationTimer_ = 0;

	bool isHit_ = false;

	bool isAttack_ = false;
};

