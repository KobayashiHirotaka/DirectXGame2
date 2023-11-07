#include "Goal.h"

void Goal::Initialize(Model* model, const Vector3& position)
{
	assert(model);
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation = position;

	SetCollisionAttribute(kCollisionAttributeGoal);
	SetCollisionMask(kCollisionMaskGoal);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
}

void Goal::Update()
{
	worldTransform_.UpdateMatrix();
}

void Goal::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Goal::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}

void Goal::OnCollision(Collider* collider)
{

}

Vector3 Goal::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}
