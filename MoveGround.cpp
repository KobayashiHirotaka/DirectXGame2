#include "MoveGround.h"
#include <cassert>

void MoveGround::Initialize(Model* model, const Vector3& position)
{
	assert(model);
	model_ = model;
	worldTransform_.translation = position;
	worldTransform_.Initialize();

	move_ = { 0.0f,0.0f,0.3f };

	stopTimer_ = 0;

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

void MoveGround::Update()
{
	worldTransform_.translation = Add(worldTransform_.translation, move_);

	if (worldTransform_.translation.z >= 50.0f)
	{
		stopTimer_++;
		move_ = { 0.0f,0.0f,0.0f };

		if (stopTimer_ >= 150)
		{
			move_ = { 0.0f,0.0f,-0.3f }; 
			stopTimer_ = 0;
		}
	}

	if (worldTransform_.translation.z < 20.0f)
	{
		stopTimer_++;
		move_ = { 0.0f,0.0f,0.0f };

		if (stopTimer_ >= 150)
		{
		    move_ = { 0.0f,0.0f,0.3f };
		    stopTimer_ = 0;
		}
	}

	worldTransform_.UpdateMatrix();
}

void MoveGround::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

void MoveGround::OnCollision(Collider* collider)
{
	
}

Vector3 MoveGround::GetWorldPosition()
{
	Vector3 pos{};
	pos.x = worldTransform_.matWorld.m[3][0];
	pos.y = worldTransform_.matWorld.m[3][1];
	pos.z = worldTransform_.matWorld.m[3][2];
	return pos;
}