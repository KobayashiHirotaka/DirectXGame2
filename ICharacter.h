#pragma once
#include "Model.h"
#include <vector>
#include "WorldTransform.h"
#include "ViewProjection.h"

enum ModelParts
{
	kModelIndexBody,
	kModelIndexHead,
	kModelIndexL_arm,
	kModelIndexR_arm,
	kModelIndexHammer
};

class ICharacter
{
public:
	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

protected:
	std::vector<Model*> models_;

	WorldTransform worldTransform_;
};