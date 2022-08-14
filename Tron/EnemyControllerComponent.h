#pragma once
#include <vector>
#include "Subject.h"
#include "ControllerComponent.h"
#include "Transform.h"


#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>


#pragma warning(pop)


class EnemyControllerComponent : public ControllerComponent, public dae::Subject
{
public:
	EnemyControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int enemyDims, glm::vec2 enemySize, glm::vec2 spawnPoint);
	~EnemyControllerComponent() override = default;
	EnemyControllerComponent(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent(EnemyControllerComponent&& other) = delete;
	EnemyControllerComponent& operator=(const EnemyControllerComponent& other) = delete;
	EnemyControllerComponent& operator=(EnemyControllerComponent&& other) = delete;

	void SetPlayerTransform(dae::Transform* playerTransform);

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

	dae::Transform* m_pPlayerTransform{};

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

	glm::vec2 m_SpawnPoint{};
};


