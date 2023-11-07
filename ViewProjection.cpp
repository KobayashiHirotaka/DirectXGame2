#include "ViewProjection.h"

void ViewProjection::Initialize() 
{
	CreateConstBuffer();
	Map();
	UpdateMatrix();
	TransferMatrix();
}

void ViewProjection::CreateConstBuffer() 
{
	constBuff = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBufferDataViewProjection));
}

void ViewProjection::Map() 
{
	constBuff.Get()->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
}

void ViewProjection::UpdateMatrix() 
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	TransferMatrix();
}

void ViewProjection::TransferMatrix()
{
	constMap->view = matView;
	constMap->projection = matProjection;
}

void ViewProjection::UpdateViewMatrix()
{
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotation, translation);
	matView = Inverse(cameraMatrix);
}

void ViewProjection::UpdateProjectionMatrix()
{
	matProjection = MakePerspectiveFovMatrix(fovAngleY, aspectRatio, nearZ, farZ);
}
