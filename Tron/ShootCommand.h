#pragma once
#include <Command.h>
#include "PlayerControllerComponent.h"

class ShootCommand final : public dae::Command
{
public:
	ShootCommand(PlayerControllerComponent* playerController);
	~ShootCommand() override = default;
	void Execute() override;

private:
	PlayerControllerComponent* m_PlayerController{};
};

