#include "PlayerControllerComponent.h"

#include <iostream>

#include "ArmComponent.h"
#include "EventQueue.h"
#include "GameObject.h"
#include "InputManager.h"
#include "MoveArmCommand.h"
#include "PlayerStateComponent.h"
#include "Renderer.h"
#include "MoveCommand.h"
#include "PlayerBulletComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ShootCommand.h"

PlayerControllerComponent::PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int playerDims, glm::vec2 playerSize, unsigned int playerIdx, glm::vec2 spawnPoint, Mode mode)
	: ControllerComponent(pLevelIndices, pLevelIndicesWalls, playerDims, playerSize)
{
	dae::EventQueue::GetInstance().Subscribe("HitPlayer0", this);
	dae::EventQueue::GetInstance().Subscribe("HitPlayer1", this);
	dae::EventQueue::GetInstance().Subscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Subscribe("LoadNextLevel", this);

	m_GameMode = mode;
	//std::cout << "subbed " << std::endl;
	m_PlayerIndex = playerIdx;
	m_SpawnPoint = spawnPoint;

	//Get the image
	m_ArmDegrees = 0;
	m_PreviousDirections = MoveDirections::Right;

	if (!(m_GameMode == Mode::Versus && m_PlayerIndex == 1))
	{
		const auto gObject = std::make_shared<dae::GameObject>();
		gObject->AddComponent(new ArmComponent(m_Dims, m_Size, &m_ArmDegrees, &m_PreviousDirections));
		dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
		m_ArmComponent = gObject;
	}
	m_CanShoot = true;
}

PlayerControllerComponent::~PlayerControllerComponent()
{

}

void PlayerControllerComponent::SetAllEnemies(std::vector<std::shared_ptr<dae::GameObject>>* pEnemies)
{
	m_pTanks = pEnemies;
}

void PlayerControllerComponent::RotateArm(bool Clockwise)
{
	if (m_GameMode == Mode::Versus && m_PlayerIndex == 1)
	{
		return;
	}

	if (m_IsDead)
	{
		return;
	}


	if (Clockwise)
	{
		if (m_ArmDegrees <= 0)
		{
			m_ArmDegrees = 360;
		}

		m_ArmDegrees -= 5;
	}
	else
	{
		if (m_ArmDegrees >= 360)
		{
			m_ArmDegrees = 0;
		}

		m_ArmDegrees += 5;
	}
}

void PlayerControllerComponent::ShootBullet()
{
	if (!m_CanShoot)
	{
		return;
	}

	m_CanShoot = false;


	if (m_IsDead)
	{
		return;
	}

	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new PlayerBulletComponent(m_pLevelIndicesWalls, m_pTanks, 10, m_Size, { sin(M_PI * (m_ArmDegrees + 90) / 180.0f), cos(M_PI * (m_ArmDegrees + 90) / 180.0f) }, { m_pOwner->GetTransform().GetPosition().x + m_pOwner->GetTransform().GetRect().width / 2, m_pOwner->GetTransform().GetPosition().y + m_pOwner->GetTransform().GetRect().height / 2 }));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
}

void PlayerControllerComponent::UpdateMovement(MoveDirections dir)
{
	if (m_IsDead)
	{
		return;
	}

	switch (dir)
	{
	case MoveDirections::Left:
		UpdateLeft();
		break;
	case MoveDirections::Right:
		UpdateRight();
		break;
	case MoveDirections::Down:
		UpdateDown();
		break;
	case MoveDirections::Up:
		UpdateUp();
		break;
	}
}

bool PlayerControllerComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "RestartLevel")
	{
		Unsubscribe();

	}
	if (event->Message == "LoadNextLevel")
	{
		Unsubscribe();
	}

	if (m_IsDead || !m_CanDie)
		return false;

	if (m_PlayerIndex == 0)
	{
		if (event->Message == "HitPlayer0")
		{
			//std::cout << "died0\n";

			m_IsDead = true;
			//std::cout << "unsubbed " << std::endl;

			//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer0", this);
			//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer1", this);
			
		}
	}
	else if (m_PlayerIndex == 1)
	{
		if (event->Message == "HitPlayer1")
		{
			//std::cout << "died1\n";
			if (m_GameMode == Mode::Versus)
			{
				m_AmountOfHits++;
				if (m_AmountOfHits >= 3)
				{
					m_IsDead = true;
				}
			}
			else
			{
				m_IsDead = true;
			}
			//std::cout << "unsubbed " << std::endl;

			//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer0", this);
			//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer1", this);
		}
	}	

	return false;
}

void PlayerControllerComponent::Unsubscribe()
{
	//std::cout << "unsubbed 1" << std::endl;
	dae::EventQueue::GetInstance().Unsubscribe("HitPlayer0", this);
	dae::EventQueue::GetInstance().Unsubscribe("HitPlayer1", this);
	dae::EventQueue::GetInstance().Unsubscribe("RestartLevel", this);
	dae::EventQueue::GetInstance().Unsubscribe("LoadNextLevel", this);
}

void PlayerControllerComponent::Startup()
{
	AddInput();
	m_pOwner->GetTransform().SetPosition(m_SpawnPoint.x - CalculateBox().width / 2, m_SpawnPoint.y - CalculateBox().height / 2, 0 );
	AddObserver(m_pOwner->GetComponentOfType<PlayerStateComponent>());
	m_pOwner->GetTransform().SetRect(CalculateBox());
	m_CanDie = false;
	m_IsDead = false;
}

void PlayerControllerComponent::Update(float deltaSec)
{
	if (m_DoneUnsubscribing)
	{
		dae::EventQueue::GetInstance().Broadcast(new dae::Event("LoadNextLevel"));
		m_DoneUnsubscribing = false;
	}

	if (m_IsDead)
	{
		//std::cout << "unsubbed " << std::endl;

		//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer0", this);
		//dae::EventQueue::GetInstance().Unsubscribe("HitPlayer1", this);

		if (m_ArmComponent)
		{
			m_ArmComponent->GetComponentOfType<ArmComponent>()->TankIsKilled();
		}
		dae::SceneManager::GetInstance().GetActiveScene()->Remove(m_pOwner);

		if (m_PlayerIndex == 0)
		{
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("KilledPlayer0"));
		}
		else
		{
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("KilledPlayer1"));			
		}
	}

	HitHorizontal();
	HitVertical();
	TranslateSprite(deltaSec);
	UpdateReset();

	m_ElapsedSecInvulnerability += deltaSec;
	if (m_ElapsedSecInvulnerability >= 3)
	{
		m_CanDie = true;
	}

	if (!m_CanShoot)
	{
		m_ElapsedSecShooting += deltaSec;

		if (m_ElapsedSecShooting >= 1.5f)
		{
			m_CanShoot = true;
			m_ElapsedSecShooting = 0.0f;
		}
	}
	if (m_ArmComponent)
	{
		m_ArmComponent->GetTransform().SetPosition(m_pOwner->GetTransform().GetPosition());
	}
}

void PlayerControllerComponent::UpdateLeft()
{
	if (m_CanMoveLeft)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingLeft"));
		m_Velocity.x = -m_MovementSpeed;
		m_PreviousDirections = MoveDirections::Left;

		if (m_GameMode == Mode::Versus && m_PlayerIndex == 1)
		{
			m_ArmDegrees = 180;
		}
	}
	else if (m_CanMoveDown || m_CanMoveUp)
	{
		switch (m_PreviousDirections)
		{
			case MoveDirections::Up: 
				UpdateUp();
				break;
			case MoveDirections::Down: 
				UpdateDown();
				break;
			default: 
				break;
		}
	}
}

void PlayerControllerComponent::UpdateRight()
{
	if (m_CanMoveRight)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingRight"));
		m_Velocity.x = m_MovementSpeed;
		m_PreviousDirections = MoveDirections::Right;

		if (m_GameMode == Mode::Versus && m_PlayerIndex == 1)
		{
			m_ArmDegrees = 0;
		}
	}
	else if (m_CanMoveDown || m_CanMoveUp)
	{
		switch (m_PreviousDirections)
		{
		case MoveDirections::Up:
			UpdateUp();
			break;
		case MoveDirections::Down:
			UpdateDown();
			break;
		default:
			break;
		}
	}
}

void PlayerControllerComponent::UpdateDown()
{
	if (m_CanMoveDown)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingDown"));
		m_Velocity.y = m_MovementSpeed;
		m_PreviousDirections = MoveDirections::Down;

		if (m_GameMode == Mode::Versus && m_PlayerIndex == 1)
		{
			m_ArmDegrees = 270;
		}
	}
	else if (m_CanMoveLeft|| m_CanMoveRight)
	{
		switch (m_PreviousDirections)
		{
		case MoveDirections::Left:
			UpdateLeft();
			break;
		case MoveDirections::Right:
			UpdateRight();
			break;
		default:
			break;
		}
	}
}

void PlayerControllerComponent::UpdateUp()
{
	if (m_CanMoveUp)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingUp"));
		m_Velocity.y = -m_MovementSpeed;
		m_PreviousDirections = MoveDirections::Up;

		if (m_GameMode == Mode::Versus && m_PlayerIndex == 1)
		{
			m_ArmDegrees = 90;
		}
	}
	else if (m_CanMoveLeft || m_CanMoveRight)
	{
		switch (m_PreviousDirections)
		{
		case MoveDirections::Left:
			UpdateLeft();
			break;
		case MoveDirections::Right:
			UpdateRight();
			break;
		default:
			break;
		}
	}
}

void PlayerControllerComponent::UpdateReset()
{
	if (m_Velocity.x == 0.0f && m_Velocity.y == 0.0f)
	{
		Notify(*m_pOwner, new dae::Event("IsIdle"));
	}

	m_Velocity.x = 0.0f;
	m_Velocity.y = 0.0f;
}

void PlayerControllerComponent::AddInput()
{
	std::cout << "addplayer" << std::endl;

	auto& input = dae::InputManager::GetInstance();
	input.SetCommandToKey(m_PlayerIndex, SDLK_a, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_d, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_s, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_w, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_e, new MoveArmCommand(this, true), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_q, new MoveArmCommand(this, false), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(m_PlayerIndex, SDLK_f, new ShootCommand(this), dae::InputManager::InputState::Pressed);


	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_DPAD_UP, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_DPAD_LEFT, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_DPAD_RIGHT, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_DPAD_DOWN, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_RIGHT_SHOULDER, new MoveArmCommand(this, true), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_LEFT_SHOULDER, new MoveArmCommand(this, false), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(m_PlayerIndex, dae::ControllerButton::GAMEPAD_A, new ShootCommand(this), dae::InputManager::InputState::Hold);
}
