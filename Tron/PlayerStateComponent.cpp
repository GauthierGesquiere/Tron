#include "PlayerStateComponent.h"

#include <iostream>

#include "EventQueue.h"
#include "InputManager.h"

#include "GameObject.h"
#include "RenderSpriteComponent.h"

PlayerStateComponent::PlayerStateComponent(unsigned displayWidth, unsigned displayHeight, unsigned int playerDims, glm::vec2 playerSize)
	: StateComponent(displayWidth, displayHeight, playerDims, playerSize)
{
}

PlayerStateComponent::~PlayerStateComponent()
{
}

void PlayerStateComponent::Startup()
{
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		const std::string fullPath{ "LeftRight.png" };

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
	case State::Dying:
		IsDying(deltaSec);
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

bool PlayerStateComponent::OnEvent(const dae::Event* event)
{
	if (event->Message == "KilledPlayer")
	{
		ElapsedSec = 0.0f;
		m_CurrentState = State::Dying;
		m_IsDead = true;
		dae::EventQueue::GetInstance().Unsubscribe("KilledPlayer", this);
	}

	return false;
}

void PlayerStateComponent::IsDrivingHorizontal(float /*deltaSec*/)
{
	//if encountered ladder and pressed up/down then is climbing
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "LeftRight.png" };

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
			const std::string fullPath{ "UpDown.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.5f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::ForwardReverseLoop, -1, m_MirrorHorizontally, m_MirrorVertically);
		}
	}
}

void PlayerStateComponent::IsWinning(float /*deltaSec*/)
{
	//next level
}

void PlayerStateComponent::IsDying(float deltaSec)
{
	ElapsedSec += deltaSec;

	//restart level
	if (const auto renderer = m_pOwner->GetComponentOfType<RenderSpriteComponent>())
	{
		if (m_PreviousState != m_CurrentState)
		{
			m_PreviousState = m_CurrentState;
			const std::string fullPath{ "Death.png" };

			renderer->SetTextureToDraw(m_SourcePath + fullPath, m_PlayerDims, m_PlayerDims, 0.3f, m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.x), m_PlayerDims * static_cast<unsigned int>(m_PlayerSize.y), LoopType::NoLoop);
		}

		if (ElapsedSec >= renderer->GetTotalAmountOfTime())
		{
			ElapsedSec = 0.0f;
			dae::EventQueue::GetInstance().Broadcast(new dae::Event("RestartLevel"));
		}
	}


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
