#include "ShootCommand.h"

ShootCommand::ShootCommand(PlayerControllerComponent* playerController)
	: m_PlayerController{ playerController }
{
}

void ShootCommand::Execute()
{
	m_PlayerController->ShootBullet();
}
