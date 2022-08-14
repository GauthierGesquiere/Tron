#include "PlayerBulletComponent.h"

PlayerBulletComponent::PlayerBulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::shared_ptr<dae::GameObject>>* pTanks, unsigned Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos)
	: BulletComponent(pLevelIndices, pTanks, Dims, Size, Directions, startPos)
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
