#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model, const Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.translation = position;

	worldTransform_.Initialize();

	SetCollisionAttribute(kCollisionAttributeGround);
	SetCollisionMask(kCollisionMaskGround);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	AABB aabbSize = 
	{
		{-10.0f,0.0f,-10.0f},
		{10.0f,0.0f,10.0f},
	};

	SetAABB(aabbSize);
}

void Ground::Update()
{
	worldTransform_.UpdateMatrix();
}

void Ground::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void Ground::OnCollision(Collider* collider) 
{
	
}

Vector3 Ground::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}
