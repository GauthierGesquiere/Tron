#pragma once
#include "Component.h"
#include "LevelsComponent.h"

class GameOverComponent : public dae::Component
{
public:
	GameOverComponent(Mode mode, unsigned int width, unsigned int height, int score);
	~GameOverComponent() override = default;
	GameOverComponent(const GameOverComponent& other) = delete;
	GameOverComponent(GameOverComponent&& other) = delete;
	GameOverComponent& operator=(const GameOverComponent& other) = delete;
	GameOverComponent& operator=(GameOverComponent&& other) = delete;

private:

};

