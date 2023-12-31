#pragma once
#include "MyMath.h"
#include "WorldTransform.h"
#include <stdint.h>
#include <algorithm>
#include "CollisionConfig.h"

class Collider
{
private:
	float radius_ = 1.0f;
	AABB aabb_ = { {-1.0f,-1.0f,-1.0f},{1.0f,1.0f,1.0f} };
	uint32_t collisionAttribute_ = 0xffffffff;
	uint32_t CollisionMask_ = 0xffffffff;
	uint32_t collisionPrimitive_ = kCollisionPrimitiveSphere;

public:
	float GetRadius() const { return radius_; }
	void SetRadius(float radius) { radius_ = radius; }
	AABB GetAABB() { return aabb_; };
	void SetAABB(AABB& aabb) { aabb_ = aabb; };
	virtual ~Collider() {}
	virtual void OnCollision(Collider* collider) = 0;
	virtual Vector3 GetWorldPosition() = 0;
	virtual WorldTransform& GetWorldTransform() = 0;
	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }
	uint32_t GetCollisionMask() const { return CollisionMask_; }
	void SetCollisionMask(uint32_t mask) { CollisionMask_ = mask; }
	uint32_t GetCollisionPrimitive() { return collisionPrimitive_; };
	void SetCollisionPrimitive(uint32_t collisionPrimitive) { collisionPrimitive_ = collisionPrimitive; };
};

