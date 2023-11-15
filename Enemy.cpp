#include "Enemy.h"
#include "Input.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

	worldTransform_.translation = { 0.0f,30.0f,250.0f };
	worldTransformHead_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformBody_.translation = { 0.0f,1.0f,0.0f };

	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation.x = 0.5f;

	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation.x = -0.5f;

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = & worldTransformBody_;

	FloatingGimmickInitialize();

	move_ = { 0.05f,0.0f,0.0f };

	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask(kCollisionMaskEnemy);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	HP_ = 5;

	AABB aabbSize =
	{
		{-1.0f,-1.0f,-1.0f},
		{1.0f,1.0f,1.0f},
	};

	SetAABB(aabbSize);
}

void Enemy::Update()
{
	ICharacter::Update();

	if (Input::GetInstance()->IsReleseKey(DIK_0)) {
		behaviorRequest_ = Behavior::kAttack;
	}

	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		// 攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}

	worldTransform_.UpdateMatrix(RotationType::Euler);

	worldTransformBody_.UpdateMatrix(RotationType::Euler);
	worldTransformHead_.UpdateMatrix(RotationType::Euler);
	worldTransformL_arm_.UpdateMatrix(RotationType::Euler);
	worldTransformR_arm_.UpdateMatrix(RotationType::Euler);
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	if (HP_ > 0) {
		models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
		models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
		models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
		models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	}
}

void Enemy::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}

void Enemy::OnCollision(Collider* collider)
{
	if (collider->GetCollisionAttribute() & kCollisionAttributePlayer)
	{
		if (HP_ > 0 && behavior_ != Behavior::kAttack) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
}

void Enemy::BehaviorRootInitialize()
{
}

void Enemy::BehaviorRootUpdate()
{
	worldTransform_.translation = Add(worldTransform_.translation, move_);

	if (worldTransform_.translation.x >= 5.0f)
	{
		move_ = { -0.05f,0.0f,0.0f };
	}

	if (worldTransform_.translation.x <= -5.0f)
	{
		move_ = { 0.05f,0.0f,0.0f };
	}

	FloatingGimmickUpdate();
}

void Enemy::BehaviorAttackInitialize()
{
	workAttack_.parameter = 0.0f;
	workAttack_.addValue = 1.0f;
}

void Enemy::BehaviorAttackUpdate()
{
	if (++workAttack_.parameter > 120) {
		behaviorRequest_ = Behavior::kRoot;
		worldTransform_.rotation.y = 0.0f;
		HP_ -= 1;
	}
	else {
		worldTransform_.rotation.y += workAttack_.addValue;
	}

}

Vector3 Enemy::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}

void Enemy::FloatingGimmickInitialize()
{
	for (int i = 0; i < kMaxModelParts; i++)
	{
		floatingParameter_[i] = 0.0f;
	}
}

void Enemy::FloatingGimmickUpdate()
{
	floatingCycle_[0] = 120;
	floatingCycle_[1] = 120;

	float step[2]{};

	for (int i = 0; i < kMaxModelParts; i++)
	{
		step[i] = 2.0f * (float)std::numbers::pi / floatingCycle_[i];

		floatingParameter_[i] += step[i];

		floatingParameter_[i] = (float)std::fmod(floatingParameter_[i], 2.0f * (float)std::numbers::pi);
	}

	worldTransformL_arm_.rotation.x = std::sin(floatingParameter_[1]) * 0.75f;
	worldTransformR_arm_.rotation.x = -std::sin(floatingParameter_[1]) * 0.75f;
}

