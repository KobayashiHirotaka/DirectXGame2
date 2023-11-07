#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "VertexData.h"
#include "Material.h"
#include <wrl.h>

class Triangle
{
public:
	void Initialize();
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection, const uint32_t textureHandle);
	void ImGui(const char* Title);

private:
	void CreateVertexBufferView();

private:
	DirectXCommon* dxCommon_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Transform tranformTriAngle_
	{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f}
	};

	Transform uvTranformTriAngle_
	{
	    {1.0f,1.0f,1.0f},
	    {0.0f,0.0f,0.0f},
	    {0.0f,0.0f,0.0f}
	};

	//バーテックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource_ = nullptr;

	//頂点データ
	VertexData* vertexData_ = nullptr;

	//バーテックスバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	//色データ
	Material* materialData_ = nullptr;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};

