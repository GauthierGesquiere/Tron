#include "PlayerBulletComponent.h"

PlayerBulletComponent::PlayerBulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos)
	: BulletComponent(pLevelIndices, Dims, Size, Directions, startPos)
{
	m_FullPath = m_SourcePath + "Player/Bullet.png";
	m_AmountOfBounces = 5;
}

void PlayerBulletComponent::Startup()
{
	BulletComponent::Startup();
}

void PlayerBulletComponent::Update(float deltaSec)
{
	BulletComponent::Update(deltaSec);
}
