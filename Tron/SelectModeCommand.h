#pragma once
#include "Command.h"
#include "PlayerSelectComponent.h"

class SelectModeCommand final : public dae::Command
{
public:
	SelectModeCommand(PlayerSelectComponent* playerSelectComponent, Directions dir);
	~SelectModeCommand() override = default;
	void Execute() override;

private:
	PlayerSelectComponent* m_PlayerSelectComponent{};
	Directions m_Direction{};
};
