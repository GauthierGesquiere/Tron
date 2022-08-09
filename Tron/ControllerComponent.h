#pragma once
#include "Component.h"

#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "utils.h"

enum class MoveDirections
{
	Left,
	Right,
	Up,
	Down
};

class ControllerComponent : public dae::Component
{
public:
	ControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int Dims, glm::vec2 Size);
	//PlayerControllerComponent() = default;
	~ControllerComponent() override = default;
	ControllerComponent(const ControllerComponent& other) = delete;
	ControllerComponent(ControllerComponent&& other) = delete;
	ControllerComponent& operator=(const ControllerComponent& other) = delete;
	ControllerComponent& operator=(ControllerComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;

	bool HitHorizontal();
	bool HitVertical();

	void TranslateSprite(float deltaTime) const;

	const utils::Rectf CalculateBox(glm::vec2 posToCalc = {-1, -1}) const;

	std::vector<std::vector<glm::vec2>>* m_pLevelIndices{};
	std::vector<std::vector<glm::vec2>>* m_pLevelIndicesWalls{};

	glm::vec2 m_Velocity{};
	glm::vec2 m_Size{};
	unsigned int m_Dims{};

	float m_MovementSpeed{};

	//bool m_HitHorizontal{ false };
	//bool m_HitVertical{ false };

	int modifier{ 2 };

	bool m_CanMoveRight{ false };
	bool m_CanMoveLeft{ false };
	bool m_CanMoveUp{ false };
	bool m_CanMoveDown{ false };
};

