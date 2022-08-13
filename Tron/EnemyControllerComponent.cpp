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
	m_NeededUpdate = NeedUpdate::Down;
}

void EnemyControllerComponent::Startup()
{
	m_pOwner->GetTransform().SetPosition(m_SpawnPoint.x, m_SpawnPoint.y, 0);
	AddObserver(m_pOwner->GetComponentOfType<EnemyStateComponent>());
	m_pOwner->GetTransform().SetRect(CalculateBox());
	m_IsInitialized = true;
}

void EnemyControllerComponent::Update(float deltaSec)
{

	HitHorizontal();
	HitVertical();
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
}

void EnemyControllerComponent::UpdateRight()
{
	if (m_CanMoveRight)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingRight"));
		m_Velocity.x = m_MovementSpeed;
	}
}

void EnemyControllerComponent::UpdateDown()
{
	if (m_CanMoveDown)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingDown"));
		m_Velocity.y = m_MovementSpeed;
	}
}

void EnemyControllerComponent::UpdateUp()
{
	if (m_CanMoveUp)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingUp"));
		m_Velocity.y = -m_MovementSpeed;
	}
}

void EnemyControllerComponent::UpdateAILogic(float /*deltaSec*/)
{
	switch (m_NeededUpdate)
	{
	case NeedUpdate::Left:
		UpdateLeft();
		break;
	case NeedUpdate::Right:
		UpdateRight();
		break;
	case NeedUpdate::Up:
		UpdateUp();
		break;
	case NeedUpdate::Down:
		UpdateDown();
		break;
	case NeedUpdate::None:
		break;
	}

	if (m_CanMoveLeft)
	{
		int randInt = rand() % 10;

		if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Left;
			return;
		}
	}
	if (m_CanMoveDown)
	{
		int randInt = rand() % 10;

		if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Down;
			return;
		}
	}
	if (m_CanMoveRight)
	{
		int randInt = rand() % 10;

		if (randInt < 3)
		{
			m_NeededUpdate = NeedUpdate::Right;
			return;
		}
	}
	if (m_CanMoveUp)
	{
		int randInt = rand() % 10;

		if (randInt < 3)
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