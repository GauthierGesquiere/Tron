#include "EnemyControllerComponent.h"

#include <iostream>

#include "EnemyStateComponent.h"
#include "EventQueue.h"
#include "GameObject.h"

EnemyControllerComponent::EnemyControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned enemyDims, glm::vec2 enemySize, glm::vec2 spawnPoint)
	: ControllerComponent(pLevelIndices, pLevelIndicesWalls, enemyDims, enemySize)
	, m_SpawnPoint{ spawnPoint }
{
	m_MovementSpeed = 25.0f;
}

void EnemyControllerComponent::Startup()
{
	m_pOwner->GetTransform().SetPosition(m_SpawnPoint.x, m_SpawnPoint.y, 0);
	AddObserver(m_pOwner->GetComponentOfType<EnemyStateComponent>());
	m_pOwner->GetTransform().SetRect(CalculateBox());

	HitHorizontal();
	HitVertical();
	if (m_CanMoveLeft)
	{
		m_NeededUpdate = NeedUpdate::Left;
	}
	if (m_CanMoveDown)
	{
		m_NeededUpdate = NeedUpdate::Down;
	}
	if (m_CanMoveRight)
	{
		m_NeededUpdate = NeedUpdate::Right;
	}
	if (m_CanMoveUp)
	{
		m_NeededUpdate = NeedUpdate::Up;
	}

	m_IsInitialized = true;
}

void EnemyControllerComponent::Update(float deltaSec)
{

	HitHorizontal();
	HitVertical();

	switch (m_NeededUpdate)
	{
	case NeedUpdate::Up:
		UpdateUp();
		break;
	case NeedUpdate::Down:
		UpdateDown();
		break;
	case NeedUpdate::Left:
		UpdateLeft();
		break;
	case NeedUpdate::Right:
		UpdateRight();
		break;
	case NeedUpdate::None:
		break;
	}

	UpdateAILogic(deltaSec);
	TranslateSprite(deltaSec);

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void EnemyControllerComponent::UpdateLeft()
{
	if (m_CanMoveLeft)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingLeft"));
		m_Velocity.x = -m_MovementSpeed;
	}
	else
	{
		std::cout << "left" << std::endl;
	}
}

void EnemyControllerComponent::UpdateRight()
{
	if (m_CanMoveRight)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingRight"));
		m_Velocity.x = m_MovementSpeed;
	}
	else
	{
		std::cout << "right" << std::endl;
	}
}

void EnemyControllerComponent::UpdateDown()
{
	if (m_CanMoveDown)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingDown"));
		m_Velocity.y = m_MovementSpeed;
	}
	else
	{
		std::cout << "down" << std::endl;
	}
}

void EnemyControllerComponent::UpdateUp()
{
	if (m_CanMoveUp)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingUp"));
		m_Velocity.y = -m_MovementSpeed;
	}
	else
	{
		std::cout << "up" << std::endl;
	}
}

void EnemyControllerComponent::UpdateAILogic(float deltaSec)
{
	if (!m_CheckAILogic)
	{
		m_ElapsedSec += deltaSec;

		if (m_ElapsedSec >= 0.5f)
		{
			m_ElapsedSec = 0.0f;
			m_CheckAILogic = true;
		}
		else
		{
			return;
		}
	}

	if (m_Velocity.x + m_Velocity.y != 0)
	{
		if (m_CanMoveDown && m_CanMoveUp && !m_CanMoveLeft && !m_CanMoveRight)
		{
			return;
		}
		if (!m_CanMoveDown && !m_CanMoveUp && m_CanMoveLeft && m_CanMoveRight)
		{
			return;
		}
	}

	m_CheckAILogic = false;

	const int randInt = rand() % 10;
	switch (m_NeededUpdate)
	{
	case NeedUpdate::Up:
		if (m_CanMoveUp)
		{
			if (randInt < 6)
			{
				m_NeededUpdate = NeedUpdate::Up;
				return;
			}
		}
		break;
	case NeedUpdate::Down:
		if (m_CanMoveDown)
		{
			if (randInt < 6)
			{
				m_NeededUpdate = NeedUpdate::Down;
				return;
			}
		}
		break;
	case NeedUpdate::Left:
		if (m_CanMoveLeft)
		{
			if (randInt < 6)
			{
				m_NeededUpdate = NeedUpdate::Left;
				return;
			}
		}
		break;
	case NeedUpdate::Right:
		if (m_CanMoveRight)
		{
			if (randInt < 6)
			{
				m_NeededUpdate = NeedUpdate::Right;
				return;
			}
		}
		break;
	case NeedUpdate::None:
		break;
	}


	if (m_CanMoveLeft)
	{
		int randInt = rand() % 10;

		if (m_NeededUpdate == NeedUpdate::Right)
		{
			if (randInt < 1)
			{
				m_NeededUpdate = NeedUpdate::Left;
				return;
			}
		}
		else if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Left;
			return;
		}
	}
	if (m_CanMoveDown)
	{
		int randInt = rand() % 10;

		if (m_NeededUpdate == NeedUpdate::Up)
		{
			if (randInt < 1)
			{
				m_NeededUpdate = NeedUpdate::Down;
				return;
			}
		}
		else if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Down;
			return;
		}
	}
	if (m_CanMoveRight)
	{
		int randInt = rand() % 10;

		if (m_NeededUpdate == NeedUpdate::Left)
		{
			if (randInt < 1)
			{
				m_NeededUpdate = NeedUpdate::Right;
				return;
			}
		}
		else if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Right;
			return;
		}
	}
	if (m_CanMoveUp)
	{
		int randInt = rand() % 10;

		if (m_NeededUpdate == NeedUpdate::Down)
		{
			if (randInt < 1)
			{
				m_NeededUpdate = NeedUpdate::Up;
				return;
			}
		}
		else if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Up;
			return;
		}
	}
	if (m_CanMoveLeft)
	{
		m_NeededUpdate = NeedUpdate::Left;
		return;
	}
	if (m_CanMoveDown)
	{
		m_NeededUpdate = NeedUpdate::Down;
		return;
	}
	if (m_CanMoveRight)
	{
		m_NeededUpdate = NeedUpdate::Right;
		return;
	}
	if (m_CanMoveUp)
	{
		m_NeededUpdate = NeedUpdate::Up;
	}
}