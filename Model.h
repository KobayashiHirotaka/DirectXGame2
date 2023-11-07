#pragma once
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyImGui.h"
#include "Light.h"
#include "VertexData.h"
#include "ModelData.h"
#include "Material.h"
#include <d3d12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <wrl.h>

class Model
{
public:
	void Initialize(const std::string& directoryPath, const std::string& filename);
	void Draw(const WorldTransform& transform, const ViewProjection& viewProjection);
	void ImGui(const char* Title);

	static Model* CreateModelFromObj(const std::string& directoryPath, const std::string& filename);

	ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);

	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

private:
	DirectXCommon* dxCommon_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Light* light_ = nullptr;

	int32_t lightFlag = Lighting::checkLighting;
	
	ModelData modelData_;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceObj_;

	VertexData* vertexDataObj_;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewObj_{};

	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceObj_ = nullptr;

	Material* materialDataObj_ = nullptr;

	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
};

