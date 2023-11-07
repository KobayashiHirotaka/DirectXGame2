#include "Model.h"
#include <cassert>

void Model::Initialize(const std::string& directoryPath, const std::string& filename)
{
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ =TextureManager::GetInstance();
	light_ = Light::GetInstance();

	materialResourceObj_ = dxCommon_->CreateBufferResource(sizeof(Material));

	modelData_ = LoadObjFile(directoryPath, filename);

	vertexResourceObj_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataObj_));
	std::memcpy(vertexDataObj_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());

	materialResourceObj_.Get()->Map(0, nullptr, reinterpret_cast<void**>(&materialDataObj_));

	materialDataObj_->enableLighting = lightFlag;
	materialDataObj_->color = color_;
	materialDataObj_->uvTransform = MakeIdentity4x4();
}

void Model::Draw(const WorldTransform& transform, const ViewProjection& viewProjection)
{
	dxCommon_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dxCommon_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewObj_);

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(1,transform.constBuff->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(4,viewProjection.constBuff->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResourceObj_->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->SetGraphicsRootDescriptorTable(2, textureManager_->GetGPUHandle(modelData_.TextureIndex));

	dxCommon_->GetcommandList()->SetGraphicsRootConstantBufferView(3, light_->GetDirectionalLight()->GetGPUVirtualAddress());

	dxCommon_->GetcommandList()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

void Model::ImGui(const char* Title)
{
	/*ImGui::Begin("Obj");
	float ImGuiScaleObj[3] = { transformObj_.scale.x,transformObj_.scale.y ,transformObj_.scale.z };
	ImGui::SliderFloat3("ScaleObj", ImGuiScaleObj, 1, 30, "%.3f");
	transformObj_.scale = { ImGuiScaleObj[0],ImGuiScaleObj[1],ImGuiScaleObj[2] };
	float ImGuiRotateObj[3] = { transformObj_.rotate.x,transformObj_.rotate.y ,transformObj_.rotate.z };
	ImGui::SliderFloat3("RotateObj", ImGuiRotateObj, -7, 7, "%.3f");
	transformObj_.rotate = { ImGuiRotateObj[0],ImGuiRotateObj[1],ImGuiRotateObj[2] };
	float ImGuiTranslateObj[3] = { transformObj_.translate.x,transformObj_.translate.y ,transformObj_.translate.z };
	ImGui::SliderFloat3("TranslateObj", ImGuiTranslateObj, -10, 10, "%.3f");
	transformObj_.translate = { ImGuiTranslateObj[0],ImGuiTranslateObj[1],ImGuiTranslateObj[2] };
	ImGui::End();*/
}

Model* Model::CreateModelFromObj(const std::string& directoryPath, const std::string& filename)
{
	Model* model = new Model();
	model->Initialize(directoryPath, filename);
	return model;
}

ModelData Model::LoadObjFile(const std::string& directoryPath, const std::string& filename)
{
	ModelData modelData;
	std::vector<Vector4>positions;
	std::vector<Vector2>texcoords;
	std::vector<Vector3>normals;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);

		s >> identifier;

		if (identifier == "v")
		{
			Vector4 position;
			s >> position.x >> position.y >> position.w;

			position.w *= -1.0f;
			position.h = 1.0f;
			positions.push_back(position);

		}
		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);

		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;

			normal.z *= -1.0f;
			normals.push_back(normal);

		}
		else if (identifier == "f")
		{
			VertexData triamgle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);

				uint32_t elementIndices[3];

				for (int32_t element = 0; element < 3; ++element)
				{
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				triamgle[faceVertex] = { position,texcoord,normal };

			}

			modelData.vertices.push_back(triamgle[2]);
			modelData.vertices.push_back(triamgle[1]);
			modelData.vertices.push_back(triamgle[0]);
		}
		else if (identifier == "mtllib")
		{
			std::string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}

	modelData.TextureIndex = textureManager_->LoadTexture(modelData.material.textureFilePath);

	vertexResourceObj_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj_.BufferLocation = vertexResourceObj_->GetGPUVirtualAddress();
	vertexBufferViewObj_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferViewObj_.StrideInBytes = sizeof(VertexData);

	return modelData;
}

MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;
	std::string line;

	std::ifstream file(directoryPath + '/' + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd")
		{
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}

	return materialData;
}
