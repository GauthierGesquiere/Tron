#pragma once
#include <Component.h>
#include <memory>
#include <string>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>

#include "ControllerComponent.h"
#pragma warning(pop)

class ArmComponent : public dae::Component
{
public:
	ArmComponent(unsigned int Dims, glm::vec2 Size, int* degrees, const MoveDirections* moveDirections);
	~ArmComponent() override = default;
	ArmComponent(const ArmComponent& other) = delete;
	ArmComponent(ArmComponent&& other) = delete;
	ArmComponent& operator=(const ArmComponent& other) = delete;
	ArmComponent& operator=(ArmComponent&& other) = delete;

protected:
	void Startup() override;
	void Update(float deltaSec) override;

private:
	std::string m_SourcePath{};
	std::shared_ptr<dae::GameObject> m_RenderObj{};

	glm::vec3 m_Offset{};

	unsigned int m_Dims{};
	glm::vec2 m_Size{};
	int* m_Degrees{};

	bool m_FlipHorizontally{ false };
	bool m_FlipVertically{ false };

	const MoveDirections* m_MoveDirection{};
};

