#include "NextSceneCommand.h"

#include <utility>

#include "SceneManager.h"

NextSceneCommand::NextSceneCommand(PlayerSelectComponent* p)
	: m_PlayerSelectMode {p}
{
}

void NextSceneCommand::Execute()
{
	m_PlayerSelectMode->StartGame();
}
