#include "NextLevelCommand.h"

#include "EventQueue.h"

NextLevelCommand::NextLevelCommand()
{
}

void NextLevelCommand::Execute()
{
	dae::EventQueue::GetInstance().Broadcast(new dae::Event("LoadNextLevel"));
}
