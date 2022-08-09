#pragma once
#include "Component.h"

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include <memory>
#include <string>
#include "utils.h"

class BulletComponent : public dae::Component
{
public:
	BulletComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, unsigned int Dims, glm::vec2 Size, glm::vec2 Directions, glm::vec2 startPos);
	BulletComponent() = default;
	~BulletComponent() override = default;
	BulletComponent(const BulletComponent& other) = delete;
	BulletComponent(BulletComponent&& other) = delete;
	BulletComponent& operator=(const BulletComponent& other) = delete;
	BulletComponent& operator=(BulletComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;

	std::string m_SourcePath{};
	std::string m_FullPath{};
	int m_AmountOfBounces{};

private:
	float m_Speed{};
	glm::vec2 m_Velocity{};
	unsigned int m_Dims{};
	glm::vec2 m_Size{};
	glm::vec2 m_StartPos{};

	std::shared_ptr<dae::GameObject> m_RenderObj{};

	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};

	void TranslateSprite(float deltaTime) const;

	bool CheckHitWall();
	const utils::Rectf CalculateBox() const;

	float ElapsedSec{};
	bool HitWall{ false };
};
