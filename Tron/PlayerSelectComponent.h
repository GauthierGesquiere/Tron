#pragma once
#include <memory>

#include "LevelsComponent.h"
#include "Component.h"
#include "Event.h"
#include "EventListener.h"

enum class Directions
{
	Up,
	Down
};


class PlayerSelectComponent : public dae::Component, public dae::EventListener
{
public:
	PlayerSelectComponent(unsigned int width, unsigned int height);
	~PlayerSelectComponent() override;
	PlayerSelectComponent(const PlayerSelectComponent& other) = delete;
	PlayerSelectComponent(PlayerSelectComponent&& other) = delete;
	PlayerSelectComponent& operator=(const PlayerSelectComponent& other) = delete;
	PlayerSelectComponent& operator=(PlayerSelectComponent&& other) = delete;

	bool OnEvent(const dae::Event* event) override;
	void StartGame();
	void ChangeMode(Directions dir);

private:
	void Startup() override;
	void Update(float deltaSec) override;

	void AddInput();

	unsigned int m_WindowWidth;
	unsigned int m_WindowHeight{};

	Mode m_CurrentMode{ Mode::Singleplayer };
	std::shared_ptr<dae::GameObject> m_Selection{};
};



