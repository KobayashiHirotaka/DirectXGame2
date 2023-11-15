#include "ICharacter.h"

void ICharacter::Initialize(const std::vector<Model*>& models)
{
	models_ = models;
	worldTransform_.Initialize();
}

void ICharacter::Update()
{
	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void ICharacter::Draw(const ViewProjection& viewProjection)
{
	for (Model* model : models_)
	{
		model->Draw(worldTransform_, viewProjection);
	}
}