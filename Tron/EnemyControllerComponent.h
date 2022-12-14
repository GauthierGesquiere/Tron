#pragma once
#include <vector>
#include "Subject.h"
#include "ControllerComponent.h"
#include "Transform.h"


#pragma warning(push)
#pragma warning (disable:4201)
#include <memory>
#include <glm/glm.hpp>

#include "EnemyStateComponent.h"



#pragma warning(pop)


class EnemyControllerComponent : public ControllerComponent, public dae::Subject
{
public:
	EnemyControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int enemyDims, glm::vec2 enemySize, glm::vec2 spawnPoint, EnemyType type);
	~EnemyControllerComponent() override = default;
	EnemyControllerComponent(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent(EnemyControllerComponent&& other) = delete;
	EnemyControllerComponent& operator=(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent& operator=(EnemyControllerComponent&& other) = delete;

	void SetPlayerTransform(std::vector<std::shared_ptr<dae::GameObject>> players);
	void SetAllEnemies(std::vector<std::shared_ptr<dae::GameObject>>* pEnemies);
	void IsHit();
	bool IsDead() const { return m_Health <= 0; }

	bool m_IsInitialized{};

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void UpdateLeft();
	void UpdateRight();
	void UpdateDown();
	void UpdateUp();

	void UpdateAILogic(float deltaSec);
	void ShootBullet();
	void CheckIfNeedsToShootBullet(float deltaSec);
	void CheckIfHitsTank(float deltaSec);

	std::vector<dae::Transform*> m_pPlayerTransforms{};


	enum class NeedUpdate
	{
		Up,
		Down,
		Left,
		Right,
		None
	};

	NeedUpdate m_NeededUpdate{};

	float m_ElapsedSecShoot{};
	bool m_JustShot{};

	float m_ElapsedSecAILogic{};
	bool m_CheckAILogic{};

	std::vector<std::shared_ptr<dae::GameObject>>* m_pAllTanks{};
	std::vector<std::shared_ptr<dae::GameObject>> m_pPlayerTanks{};

	glm::vec2 m_SpawnPoint{};

	int m_Health{3};

	EnemyType m_Type{};
};


