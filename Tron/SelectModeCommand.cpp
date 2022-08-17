#include "SelectModeCommand.h"

SelectModeCommand::SelectModeCommand(PlayerSelectComponent* playerSelectComponent, Directions dir)
	: m_PlayerSelectComponent{ playerSelectComponent }
	, m_Direction{ dir }
{
}

void SelectModeCommand::Execute()
{
	m_PlayerSelectComponent->ChangeMode(m_Direction);
}
