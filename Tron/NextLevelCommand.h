#pragma once
#include "Command.h"
#include "PlayerControllerComponent.h"

class NextLevelCommand final : public dae::Command
{
public:
	NextLevelCommand();
	~NextLevelCommand() override = default;
	void Execute() override;;
};

