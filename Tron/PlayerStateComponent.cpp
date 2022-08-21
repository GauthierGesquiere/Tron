#include "PlayerStateComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "InputManager.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

PlayerStateComponent::PlayerStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned int playerDims, glm::vec2 playerSize, int playerIdx, Mode mode)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
{
	m_PlayerIdx = playerIdx;
	m_GameMode = mode;
}

PlayerStateComponent::~PlayerStateComponent()
{
}

void PlayerStateComponent::Startup()
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		std::string fullPath{ "LeftRight" };
		if (m_GameMode == Mode::Versus && m_PlayerIdx == 1)
		{
			fullPath += std::to_string(m_PlayerIdx + 1) + ".png";
		}
		else
		{
			fullPath += std::to_string(m_PlayerIdx) + ".png";
		}

		renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally);
		renderer->SetFreeze(2);
	}
}

void PlayerStateComponent::Update(float deltaSec)
{
	switch (m_CurrentState)
	{
	case State::DrivingHorizontal:
		IsDrivingHorizontal(deltaSec);
		break;
	case State::DrivingVertical:
		IsDrivingVertical(deltaSec);
		break;
	case State::Winning:
		IsWinning(deltaSec);
		break;
	case State::Idle:
		IsIdle(deltaSec);
		break;
	case State::Nothing: 
		break;
	default: 
		break;
	}
}

void PlayerStateComponent::OnNotify(const dae::GameObject& /*actor*/, dae::Event* event)
{
	if (m_IsDead)
	{
		delete event;
		return;
	}

	if (event->Message == "IsDrivingRight")
	{
		m_CurrentState = State::DrivingHorizontal;

		//Was walking to the Right?
		if (m_MirrorHorizontally == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorHorizontally = false;
	}
	else if (event->Message == "IsDrivingLeft")
	{
		m_CurrentState = State::DrivingHorizontal;

		//Was walking to the Left?
		if ((m_MirrorHorizontally == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorHorizontally = true;
	}
	else if (event->Message == "IsDrivingUp")
	{
		m_CurrentState = State::DrivingVertical;

		//Was ClimbingUp?
		if (m_MirrorVertically == true)
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorVertically = false;
	}
	else if (event->Message == "IsDrivingDown")
	{
		m_CurrentState = State::DrivingVertical;

		//Was ClimbingDown?
		if ((m_MirrorVertically == false))
		{
			//ResetState
			m_PreviousState = State::Nothing;
		}
		m_MirrorVertically = true;
	}
	else if(event->Message == "IsIdle")
	{
		if (m_CurrentState != State::Idle)
		{
			//std::cout << "Idle" << std::endl;
			m_PreviousStateBeforeIdle = m_CurrentState;
			m_CurrentState = State::Idle;
		}
	}
	//Important
	delete event;
}

void PlayerStateComponent::IsDrivingHorizontal(float /*deltaSec*/)
{
	//if encountered ladder and pressed up/down then is climbing
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			std::string fullPath{ "LeftRight" };

			if (m_GameMode == Mode::Versus && m_PlayerIdx == 1)
			{
				fullPath += std::to_string(m_PlayerIdx + 1) + ".png";
			}
			else
			{
				fullPath += std::to_string(m_PlayerIdx) + ".png";
			}

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally);
		}
	}
}

void PlayerStateComponent::IsDrivingVertical(float /*deltaSec*/)
{
	//if encountered ladder and pressed up/down then is climbing
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			std::string fullPath{ "UpDown" };

			if (m_GameMode == Mode::Versus && m_PlayerIdx == 1)
			{
				fullPath += std::to_string(m_PlayerIdx + 1) + ".png";
			}
			else
			{
				fullPath += std::to_string(m_PlayerIdx) + ".png";
			}

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally, m_MirrorVertically);
		}
	}
}

void PlayerStateComponent::IsWinning(float /*deltaSec*/)
{
	//next level
}

void PlayerStateComponent::IsIdle(float /*deltaSec*/)
{
	//wait 0.1f then back to previous state
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			//const std::string fullPath{ "Idle.png" };

			//if (m_PreviousState == State::Climbing)
			//{
				renderer->SetFreeze();
			//}
			//else
			//{
				//renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 1.0f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
			//}
			m_PreviousState = m_CurrentState;
		}
	}
}
