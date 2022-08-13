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

PlayerControllerComponent::PlayerControllerComponent(std::vector<std::vector<glm::vec2>>* pLevelIndices, std::vector<std::vector<glm::vec2>>* pLevelIndicesWalls, unsigned int playerDims, glm::vec2 playerSize)
	: ControllerComponent(pLevelIndices, pLevelIndicesWalls, playerDims, playerSize)
{
	dae::EventQueue::GetInstance().Subscribe("KilledPlayer", this);

	//Get the image
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new ArmComponent(m_Dims, m_Size, &m_ArmDegrees, &m_PreviousDirections));
	dae::SceneManager::GetInstance().GetActiveScene()->Add(gObject);
	m_ArmComponent = gObject;
}

PlayerControllerComponent::~PlayerControllerComponent()
{
	 dae::InputManager::GetInstance().RemoveCommands();
}

void PlayerControllerComponent::RotateArm(bool Clockwise)
{
	if (Clockwise)
	{
		if (m_ArmDegrees <= 0)
		{
			m_ArmDegrees = 360;
		}

		m_ArmDegrees -= 10;
	}
	else
	{
		if (m_ArmDegrees >= 360)
		{
			m_ArmDegrees = 0;
		}

		m_ArmDegrees += 10;
	}
}

void PlayerControllerComponent::ShootBullet()
{
	const auto gObject = std::make_shared<dae::GameObject>();
	gObject->AddComponent(new PlayerBulletComponent(m_pLevelIndicesWalls, 10, m_Size, { sin(M_PI * (m_ArmDegrees + 90) / 180.0f), cos(M_PI * (m_ArmDegrees + 90) / 180.0f) }, { m_pOwner->GetTransform().GetPosition().x + m_pOwner->GetTransform().GetRect().width / 2, m_pOwner->GetTransform().GetPosition().y + m_pOwner->GetTransform().GetRect().height / 2 }));
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
	if (event->Message == "KilledPlayer")
	{
		m_IsDead = true;
		dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer", this);
	}
	return false;
}

void PlayerControllerComponent::Startup()
{
	AddInput();
	m_pOwner->GetTransform().SetPosition( 161,  156, 0 );
	AddObserver(m_pOwner->GetComponentOfType<PlayerStateComponent>());
	m_pOwner->GetTransform().SetRect(CalculateBox());
}

void PlayerControllerComponent::Update(float deltaSec)
{
	HitHorizontal();
	HitVertical();
	TranslateSprite(deltaSec);
	UpdateReset();

	m_ArmComponent->GetTransform().SetPosition(m_pOwner->GetTransform().GetPosition());
}

void PlayerControllerComponent::UpdateLeft()
{
	if (m_CanMoveLeft)
	{
		Notify(*m_pOwner, new dae::Event("IsDrivingLeft"));
		m_Velocity.x = -m_MovementSpeed;
		m_PreviousDirections = MoveDirections::Left;
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
	auto& input = dae::InputManager::GetInstance();
	input.SetCommandToKey(0, SDLK_a, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_d, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_s, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_w, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	input.SetCommandToKey(0, SDLK_e, new MoveArmCommand(this, true), dae::InputManager::InputState::Pressed);
	input.SetCommandToKey(0, SDLK_q, new MoveArmCommand(this, false), dae::InputManager::InputState::Pressed);
	input.SetCommandToKey(0, SDLK_f, new ShootCommand(this), dae::InputManager::InputState::Pressed);


	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_UP, new MoveCommand(this, MoveDirections::Up), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_LEFT, new MoveCommand(this, MoveDirections::Left), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_RIGHT, new MoveCommand(this, MoveDirections::Right), dae::InputManager::InputState::Hold);
	input.SetCommandToButton(0, dae::ControllerButton::GAMEPAD_DPAD_DOWN, new MoveCommand(this, MoveDirections::Down), dae::InputManager::InputState::Hold);
}
