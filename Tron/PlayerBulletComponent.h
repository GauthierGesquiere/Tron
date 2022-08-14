#pragma once
#include "BulletComponent.h"
class PlayerBulletComponent : public BulletComponent
{
public:
	PlayerBulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::shared_ptr<dae::GameObject>>* pTanks, unsigned int Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos);
	PlayerBulletComponent() = default;
	~PlayerBulletComponent() override = default;
	PlayerBulletComponent(const PlayerBulletComponent& other) = delete;
	PlayerBulletComponent(PlayerBulletComponent&& other) = delete;
	PlayerBulletComponent& operator=(const PlayerBulletComponent& other) = delete;
	PlayerBulletComponent& operator=(PlayerBulletComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;

};

