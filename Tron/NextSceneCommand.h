#pragma once
#include <string>
#include "PlayerSelectComponent.h"

#include "Command.h"

class NextSceneCommand final : public dae::Command
{
public:
	NextSceneCommand(PlayerSelectComponent* p);
	~NextSceneCommand() override = default;
	void Execute() override;

private:
	PlayerSelectComponent* m_PlayerSelectMode{};
};

