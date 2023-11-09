#pragma once
#include "IScene.h"
#include "MyEngine.h"
#include "Camera.h"
#include "Light.h"
#include "Triangle.h"
#include "Sprite.h"
#include "Sphere.h"
#include "Model.h"
#include "ModelData.h"
#include "Input.h"
#include "Audio.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include "CollisionManager.h"

#include "Player.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Goal.h"
#include "FollowCamera.h"
#include "Ground.h"
#include "MoveGround.h"
#include "Skydome.h"


#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class GamePlayScene : public IScene
{
public:
	GamePlayScene();
	~GamePlayScene();

	void Initialize();
	void Update();
	void Draw();

private:
	MyEngine* engine_ = nullptr;

	Camera* camera_ = nullptr;

	//std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	TextureManager* textureManager_ = nullptr;

	Light* light_ = nullptr;

	Input* input_ = nullptr;

	Audio* audio_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

	uint32_t sound_;

	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;

	std::unique_ptr<Model> weaponModel_;

	std::unique_ptr<Enemy> enemy_;
	std::unique_ptr<Model> enemyModel_;

	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Goal> goal_[2];
	std::unique_ptr<Model> goalModel_;
	static const uint32_t groundNum_ = 16;
	std::unique_ptr<Ground> ground_[groundNum_];
	std::unique_ptr<MoveGround> moveGround_;
	std::unique_ptr<Model> groundModel_;
	std::unique_ptr<Model> groundRedModel_;

	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;

	bool isDebugCameraActive_ = false;
};
