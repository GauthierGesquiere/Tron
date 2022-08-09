#pragma once
#include <Command.h>
#include "PlayerControllerComponent.h"

class MoveArmCommand : public dae::Command
{
public:
	MoveArmCommand(PlayerControllerComponent* playerController, bool Clockwise);
	~MoveArmCommand() override = default;
	void Execute() override;

private:
	PlayerControllerComponent* m_PlayerController{};
	bool m_Clockwise{};
};

