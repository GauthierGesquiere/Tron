#pragma once
#include <vector>

#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include <memory>

#include "ControllerComponent.h"
#include "EventListener.h"
#include "Subject.h"




class PlayerControllerComponent : public ControllerComponent, public dae::Subject, public dae::EventListener
{
public:
	PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int playerDims, glm::vec2 playerSize);
	//PlayerControllerComponent() = default;
	~PlayerControllerComponent() override;
	PlayerControllerComponent(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent(PlayerControllerComponent&& other) = delete;
	PlayerControllerComponent& operator=(const PlayerControllerComponent& other) = delete;
	PlayerControllerComponent& operator=(PlayerControllerComponent&& other) = delete;

	void RotateArm(bool Clockwise);
	void ShootBullet();
	void UpdateMovement(MoveDirections dir);
	bool OnEvent(const dae::Event* event) override;

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void UpdateLeft();
	void UpdateRight();
	void UpdateDown();
	void UpdateUp();

	void UpdateReset();

	void AddInput();

	bool m_IsDead{};

	MoveDirections m_PreviousDirections{};

	int m_ArmDegrees{};

	std::shared_ptr<dae::GameObject> m_ArmComponent{};

};




