#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model)
{
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale = { 150.0f, 150.0f, 150.0f };
}

void Skydome::Updata()
{
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}
