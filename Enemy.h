#include "Collider.h"
#include "CollisionConfig.h"
#include "ICharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "MyMath.h"
#include <numbers>
#include <optional>

class Enemy : public Collider, public ICharacter
{
public:
	enum class Behavior {
		kRoot,
		kAttack
	};

	struct WorkAttack {
		float parameter;
		float addValue;
		Vector3 start_Arm;
		Vector3 end_Arm;
		Vector3 start_Weapon;
		Vector3 end_Weapon;
	};

public:
	void Initialize(const std::vector<Model*>& models)override;

	void Update()override;

	void Draw(const ViewProjection& viewProjection)override;

	WorldTransform& GetWorldTransform()override { return worldTransform_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

	Vector3 GetWorldPosition() override;

	void FloatingGimmickInitialize();

	void FloatingGimmickUpdate();

	void SetParent(const WorldTransform* parent);

	void OnCollision(Collider* collider)override;

	// 通常行動初期化
	void BehaviorRootInitialize();
	// 通常行動更新
	void BehaviorRootUpdate();
	// 攻撃行動初期化
	void BehaviorAttackInitialize();
	// 攻撃行動更新
	void BehaviorAttackUpdate();


private:
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	const uint16_t kMaxModelParts = 2;

	Vector3 move_;

	float floatingParameter_[2];

	int floatingCycle_[2];

	float floatingAmplitude_;

	uint32_t HP_;

	// パラメータ
	WorkAttack workAttack_;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
};
