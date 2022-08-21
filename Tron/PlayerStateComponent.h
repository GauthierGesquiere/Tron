#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/glm.hpp>
#pragma warning(pop)

#include "EventListener.h"
#include "LevelsComponent.h"
#include "Observer.h"
#include "StateComponent.h"

class PlayerStateComponent : public StateComponent, public dae::Observer
{
public:
	PlayerStateComponent(unsigned int displayWidth, unsigned int displayHeight, unsigned int playerDims, glm::vec2 playerSize, int playerIdx, Mode mode);
	~PlayerStateComponent() override;
	PlayerStateComponent(const PlayerStateComponent& other) = delete;
	PlayerStateComponent(PlayerStateComponent&& other) = delete;
	PlayerStateComponent& operator=(const PlayerStateComponent& other) = delete;
	PlayerStateComponent& operator=(PlayerStateComponent&& other) = delete;

	void Startup() override;
	void Update(float deltaSec) override;

	void OnNotify(const dae::GameObject& actor, dae::Event* event) override;

private:
	void IsDrivingHorizontal(float deltaSec);
	void IsDrivingVertical(float deltaSec);
	void IsWinning(float deltaSec);
	void IsIdle(float deltaSec);

	bool m_IsDead{};

	int m_PlayerIdx{};

	Mode m_GameMode{};
};





