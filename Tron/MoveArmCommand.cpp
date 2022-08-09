#include "MoveArmCommand.h"

MoveArmCommand::MoveArmCommand(PlayerControllerComponent* playerController, bool Clockwise)
	: m_PlayerController{ playerController }
	, m_Clockwise{Clockwise}
{
}

void MoveArmCommand::Execute()
{
	m_PlayerController->RotateArm(m_Clockwise);
}
