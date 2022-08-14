#include "EnemyBulletComponent.h"

EnemyBulletComponent::EnemyBulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos)
	: BulletComponent(pLevelIndices, Dims, Size, Directions, startPos)
{
	m_FullPath = m_SourcePath + "Enemies/Bullet.png";
	m_AmountOfBounces = 1;
}

void EnemyBulletComponent::Startup()
{
	BulletComponent::Startup();
}

void EnemyBulletComponent::Update(float deltaSec)
{
	BulletComponent::Update(deltaSec);
}
