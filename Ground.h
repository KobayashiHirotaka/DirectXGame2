#pragma once
#include "Collider.h"
#include "CollisionConfig.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Ground : public Collider
{
public:
	void Initialize(Model* model, const Vector3& position);

	void Update();

	void Draw(ViewProjection& viewProjection);

	WorldTransform& GetWorldTransform()override { return worldTransform_; }

	Vector3 GetWorldPosition() override;

	void OnCollision(Collider* collider)override;

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;
};

