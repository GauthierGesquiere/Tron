#pragma once
#include "BulletComponent.h"
class EnemyBulletComponent : public BulletComponent
{
public:
	EnemyBulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos);
	EnemyBulletComponent() = default;
	~EnemyBulletComponent() override = default;
	EnemyBulletComponent(const EnemyBulletComponent& other) = delete;
	EnemyBulletComponent(EnemyBulletComponent&& other) = delete;
	EnemyBulletComponent& operator=(const EnemyBulletComponent& other) = delete;
	EnemyBulletComponent& operator=(EnemyBulletComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;
};

