#include "Enemy.h"

void Enemy::Initialize(const std::vector<Model*>& models)
{
	ICharacter::Initialize(models);

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
}

void Enemy::Update()
{
	ICharacter::Update();

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

	worldTransform_.UpdateMatrix();

	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
}

void Enemy::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}

void Enemy::OnCollision(Collider* collider)
{
	
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

